#include "CollisionManager.h"
#include "../Scene/Scene.h"
#include "Collider.h"
#include "../Object/Object.h"
#include "../Object/PointObject.h"
#include "../Collider/ColliderPoint.h"
#include "../Collider/ColliderRect.h"
#include "../Resources/ResourceManager.h"
#include "../Scene/SceneManager.h"
#include "../Core/Input.h"
#include "../Core/Camera.h"
#include "../Scene/GameScene.h"
#include "CollisionSpace.h"
#include "../Application/Window.h"
#include "../utils.h"
#include "../Object/MoveObj/Player.h"
#include "../Object/StaticObj/GameManager.h"
#include "../Profiler.h"
#include "../Option.h"
#include <mutex>

int checkNum = 0;
float duration = 0.f;
mutex g_mtx;
DEFINITION_SINGLE(CollisionManager)

CollisionManager::CollisionManager()
    :
    m_pCollTex(Texture::MakeTexture())
{
    Texture* pTex = Texture::CreateEmptyTexture(WINDOW->GetWndDC(), GETRESOLUTION.x, GETRESOLUTION.y);
    m_pCollTex.reset(pTex);
    m_pCollTex->SetColorKey(util::Black);
}

CollisionManager::~CollisionManager()
{
}

bool CollisionManager::Init()
{
    return true;
}

void CollisionManager::ClickPoint()
{
    AddCollidePoint(MOUSEWORLDPOS, "Click");
}

void CollisionManager::AddCollideObject(Object* pObj)
{
    pObj->LateUpdate(0.f);
    m_tempCollisionObjList.push_back(pObj);
#ifdef QUAD_TREE
    const auto& collList = pObj->GetColliderList();
    for (auto iter = collList->begin(); iter != collList->end(); ++iter)
    {
        const auto& pColl = (*iter);
        if (pColl->GetEnable())
        {
            pColl->SetMoved(true);
            m_CollisionSpace->Observe(pColl);
        }
    }
#else
    m_CollisionObjList.push_back(pObj);
#endif
}

void CollisionManager::AddCollideRect(const Pos& pos, const Rect& rect, const string& strTag)
{
    Object* pPoint = Object::CreateObject<PointObject>(strTag);
    ColliderRect* pColl = pPoint->AddCollider<ColliderRect>(strTag);
    pColl->SetRect(rect.left, rect.top, rect.right, rect.bottom);
    pColl->SetMoved(true);
    pPoint->SetPos(pos.x, pos.y);
    pPoint->LateUpdate(0.f);
    m_tempCollisionObjList.push_back(pPoint);
#ifdef QUAD_TREE
    m_CollisionSpace->Observe(pColl);
#else
    m_CollisionObjList.push_back(pPoint);
#endif
    SAFE_RELEASE(pColl);
}

void CollisionManager::AddCollidePoint(const Pos& pos, const string& strTag)
{
    Object* pPoint = Object::CreateObject<PointObject>(strTag);
    ColliderPoint* pColl = pPoint->AddCollider<ColliderPoint>(strTag);
    pColl->SetMoved(true);
    pPoint->SetPos(pos.x, pos.y);
    pPoint->LateUpdate(0.f);
    m_tempCollisionObjList.push_back(pPoint);
#ifdef QUAD_TREE
    m_CollisionSpace->Observe(pColl);
#else
    m_CollisionObjList.push_back(pPoint);
#endif
    SAFE_RELEASE(pColl);
}

void CollisionManager::Clear()
{
    m_CollisionObjList.clear();

    // 한 프레임에 추가된 즉발 충돌체
    const auto iterEnd = m_tempCollisionObjList.end();
    for (auto iter = m_tempCollisionObjList.begin(); 
        iter != iterEnd; ++iter)
    {
        SAFE_RELEASE((*iter));
    }
    m_tempCollisionObjList.clear();
}

void CollisionManager::NaiveAdd(Object* const& pObj)
{
    m_CollisionObjList.push_back(pObj);
}

void CollisionManager::AddObject(Object* pObj)
{
    if (pObj->CheckCollider())
    {
#ifdef QUAD_TREE
        for (Collider* coll : *(pObj->GetColliderList()))
        {
            if (coll->GetEnable())
            {
                m_CollisionSpace->Observe(coll);
            }
        }
#else
        m_CollisionObjList.push_back(pObj);
#endif
    }
}

void CollisionManager::EraseCollider(Collider* pColl)
{
#ifdef QUAD_TREE
    m_CollisionSpace->ErasePreviousCollider(pColl);
#endif
}

void CollisionManager::QuadTreeUpdate()
{
    m_CollisionSpace->Update();
}

void CollisionManager::SetUpCollisionSpace()
{
    CollisionSpace::SetQuadTree();
    m_CollisionSpace = CollisionSpace::GetCurQuadTree();
}

void CollisionManager::CollisionListVersion(float dt)
{
    if (m_CollisionObjList.size() < 2)
    {
        Clear();
        return;
    }
    // 충돌 체크 성능 체크
    PROBE_PERFORMANCE("Collision/CollisionCheck");

    checkNum = 0;
    // 오브젝트 간 충돌 처리를 한다.
    list<Object*>::iterator iter;
    list<Object*>::iterator iterEnd = m_CollisionObjList.end();
    --iterEnd;
   
    // 오브젝트 기준으로 한 번 비교
    for (iter = m_CollisionObjList.begin(); iter != iterEnd; ++iter)
    {
        list<Object*>::iterator jter = iter;
        ++jter;
        list<Object*>::iterator jterEnd = m_CollisionObjList.end();
        for (; jter != jterEnd; ++jter)
        {
            // 두 오브젝트간 콜라이어들끼리 비교
            CheckCollision(*iter, *jter, dt);
        }
    }
    
    Clear();

    duration += dt;
    if (duration >= 1.0f)
    {
        duration -= 1.0f;
#ifdef _DEBUG
        _cprintf("collision check num : %d\n", checkNum);
#endif
        wchar_t message[256] = {};
        swprintf_s(message, L"collision check num : %d\n", checkNum);
        OutputDebugString(message);
    }

}

void CollisionManager::CollisionQuadTreeVersion(float dt)
{
    m_pCollTex->ClearBuffer(0, 0, GETRESOLUTION.x, GETRESOLUTION.y);

    int srcCollNum = m_CollisionSpace->GetCurSize();
    if (srcCollNum < 2)
    {
        Clear();
        return;
    }

    m_CollisionSpace->Ready();

    // 콜라이더 보여주기 On 상태인 경우
    if (SHOWCHECK(SHOW_COLL))
    {
        m_CollisionSpace->Draw(m_pCollTex->GetDC(), dt);
        const auto& pPlayerColl = m_CollisionSpace->FindCollider("PlayerBody");
        if (pPlayerColl)
        {
            vector<Collider*> pPlayerList;
            m_CollisionSpace->GetEqualSpaceColliders(pPlayerColl, pPlayerList);
            DrawColliders(pPlayerList, nullptr);
        }
    }

    // 충돌 체크 성능 체크
    checkNum = 0;
    PROBE_PERFORMANCE("Collision/CollisionCheck");

    const auto& pSrcList = *(m_CollisionSpace->GetColliderList());
    // 충돌체와 충돌체 검사 시작 부분
    for (Collider* const& pSrc : pSrcList)
    {
        // 해당 원소 비어 있는 경우
        if (pSrc == nullptr)
            continue;

        // 물체가 움직이지 않았다면 체크하지 않는다.
        if (!pSrc->IsMoved())
            continue;

        // 충돌체 검색
        vector<Collider*> pDstList;
        m_CollisionSpace->GetEqualSpaceColliders(pSrc, pDstList);

        // 충돌가능한 모든 상대방 충돌체 
        for (Collider* const& pDst : pDstList)
        {

            // 이전 충돌에서 src가 삭제되었을 때
            if (pSrc == nullptr)
                break;

            ++checkNum;

            // 비교했다는 표시를 남김
            m_CollisionSpace->Mark(pSrc, pDst);

            // 충돌체가 서로 충돌했는가
            if (pSrc->CheckCollision(pDst))
            {
                // 히트 포인트 셋팅
                pDst->SetHitPoint(pSrc->GetHitPoint());

                // 이전에 충돌하지 않았음
                if (!pSrc->CheckCollisionList(pDst))
                {
                    // 서로 상대방을 충돌 목록으로 추가한다.
                    pSrc->AddCollider(pDst);
                    pDst->AddCollider(pSrc);

                    pSrc->CallFunction(CS_ENTER, pDst, dt);
                    pDst->CallFunction(CS_ENTER, pSrc, dt);
                }
                // 한번 충돌한 적이 있음
                else
                {
                    pSrc->CallFunction(CS_STAY, pDst, dt);
                    pDst->CallFunction(CS_STAY, pSrc, dt);
                }
            }
            // 충돌하지 않았지만 이전 이력이 있는 경우
            else if (pSrc->CheckCollisionList(pDst))
            {
                // 서로 충돌이 안되므로 충돌목록에서 지워준다.
                pSrc->EraseCollisionList(pDst);
                pDst->EraseCollisionList(pSrc);
                // 충돌에서 떠나는 상태
                pSrc->CallFunction(CS_LEAVE, pDst, dt);
                pDst->CallFunction(CS_LEAVE, pSrc, dt);
            }
        }
    }

    Clear();

    duration += dt;
    if (duration >= 1.0f)
    {
        duration -= 1.0f;
#ifdef _DEBUG
        _cprintf("collision check num : %d\n", checkNum);
#endif
        wchar_t message[256] = {};
        swprintf_s(message, L"collision check num : %d\n", checkNum);
        OutputDebugString(message);
    }

}

void CollisionManager::Draw(HDC hdc, float dt)
{
    m_pCollTex->DrawImageAt(hdc, Pos(0.f, 0.f));
}

bool CollisionManager::CheckCollision(Object* pSrc, Object* pDst, float dt)
{

    const list<Collider*>* pSrcList = pSrc->GetColliderList();
    const list<Collider*>* pDstList = pDst->GetColliderList();
    const auto& iterSrcEnd = pSrcList->cend();
    const auto& iterDstEnd = pDstList->cend();

    bool bCollision = false;
    for (auto iterSrc = pSrcList->begin(); iterSrc != iterSrcEnd; ++iterSrc)
    {
        if (!(*iterSrc)->GetEnable())
        {
            continue;
        }
        for (auto iterDst = pDstList->begin(); iterDst != iterDstEnd; ++iterDst)
        {
            if (!(*iterDst)->GetEnable())
            {
                continue;
            }

            ++checkNum;

            // 충돌체가 서로 충돌했는가
            if ((*iterSrc)->CheckCollision(*iterDst))
            {
                bCollision = true;
                // 히트 포인트 셋팅
                ((*iterDst))->SetHitPoint((*iterSrc)->GetHitPoint());

                // 충돌목록에서 이전에 충돌된 적이 없다면
                // 처음 막 충돌되었다는 의미
                if (!(*iterSrc)->CheckCollisionList(*iterDst))
                {
                    // 서로 상대방을 충돌 목록으로 추가한다.
                    (*iterSrc)->AddCollider(*iterDst);
                    (*iterDst)->AddCollider(*iterSrc);

                    (*iterSrc)->CallFunction(CS_ENTER, *iterDst, dt);
                    (*iterDst)->CallFunction(CS_ENTER, *iterSrc, dt);
                }
                // 기존 충돌된 적이 있다면 계속 충돌 상태인 것이다.
                else
                {
                    (*iterSrc)->CallFunction(CS_STAY, *iterDst, dt);
                    (*iterDst)->CallFunction(CS_STAY, *iterSrc, dt);
                }
            }
            // 현재 충돌하지 않았다면,
            // 현재 충돌이 안된 상태에서 이전에 충돌이 되고 있엇
            // 다면 이제 막 충돌상태에서 떨어졌다는 의미이다.
            else if ((*iterSrc)->CheckCollisionList(*iterDst))
            {
                // 서로 충돌이 안되므로 충돌목록에서 지워준다.
                (*iterSrc)->EraseCollisionList(*iterDst);
                (*iterDst)->EraseCollisionList(*iterSrc);

                (*iterSrc)->CallFunction(CS_LEAVE, *iterDst, dt);
                (*iterDst)->CallFunction(CS_LEAVE, *iterSrc, dt);
            }
        }
    }
    return bCollision;
}

void CollisionManager::DrawColliders(const vector<Collider*> &colls, Collider* pColl)
{
    if(SHOWCHECK(SHOW_COLL))
    {
        for (Collider* coll : colls)
        {
            Rect screenRect = coll->GetBounds().SubtractOffset(CAMERA->GetTopLeft());
            DrawColorRectWithOutBorder(m_pCollTex->GetDC(), screenRect, util::Magenta);
        }
        if (pColl)
        {
            Rect screenRect = pColl->GetBounds().SubtractOffset(CAMERA->GetTopLeft());
            DrawColorRectWithOutBorder(m_pCollTex->GetDC(), screenRect, util::Magenta);
        }
    }
}
