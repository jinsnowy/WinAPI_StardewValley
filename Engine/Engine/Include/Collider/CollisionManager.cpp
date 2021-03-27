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

DEFINITION_SINGLE(CollisionManager)

size_t quad_build_total_us = 0;
size_t collidecheck_total_us = 0;
int frame_count = 0;
extern int frame_num;

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
    SAFE_RELEASE(m_pCollTex);
}

bool CollisionManager::Init()
{
    m_pCollTex = Texture::CreateEmptyTexture(WINDOW->GetWndDC(), GETRESOLUTION.x, GETRESOLUTION.y);
    m_pCollTex->SetColorKey(util::Black);
    m_CollisionSpace = make_unique<CollisionSpace>();
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
    const auto& collList = pObj->GetColliderList();
    for (auto iter = collList->begin(); iter != collList->end(); ++iter)
    {
        const auto& pColl = (*iter);
        if (pColl->GetEnable())
        {
            m_CollisionSpace->Observe(pColl);
        }
    }
}

void CollisionManager::AddCollideRect(const Pos& pos, const Rect& rect, const string& strTag)
{
    Object* pPoint = Object::CreateObject<PointObject>(strTag);
    pPoint->SetPos(pos.x, pos.y);

    ColliderRect* pColl = pPoint->AddCollider<ColliderRect>(strTag);
    pColl->SetRect(rect.left, rect.top, rect.right, rect.bottom);

    pPoint->LateUpdate(0.f);
    m_tempCollisionObjList.push_back(pPoint);
    m_CollisionSpace->Observe(pColl);
    SAFE_RELEASE(pColl);
}

void CollisionManager::AddCollidePoint(const Pos& pos, const string& strTag)
{
    Object* pPoint = Object::CreateObject<PointObject>(strTag);
    pPoint->SetPos(pos.x, pos.y);

    ColliderPoint* pColl = pPoint->AddCollider<ColliderPoint>(strTag);
    pPoint->LateUpdate(0.f);
    m_tempCollisionObjList.push_back(pPoint);
    m_CollisionSpace->Observe(pColl);
    SAFE_RELEASE(pColl);
}

void CollisionManager::Clear()
{
    m_CollisionObjList.clear();

    for (auto iter = m_tempCollisionObjList.begin();
        iter != m_tempCollisionObjList.end();
        ++iter)
    {
        SAFE_RELEASE((*iter));
    }
    m_tempCollisionObjList.clear();

    if (SHOWCHECK(SHOW_COLL))
    {
        m_CollisionSpace->Draw(m_pCollTex->GetDC(), 0.f);
    }

    // 쿼드 트리 초기화
    m_CollisionSpace->Clear();
}

void CollisionManager::NaiveAdd(Object* const& pObj)
{
    m_CollisionObjList.push_back(pObj);
}

void CollisionManager::AddObject(Object* pObj)
{
    auto tic = chrono::steady_clock::now();
    if (pObj->CheckCollider())
    {
        // NaiveAdd(pObj);
        for (Collider* coll : *(pObj->GetColliderList()))
        {
            if (coll->GetEnable())
            {
                m_CollisionSpace->Observe(coll);
            }
        }
    }
    auto toc = chrono::steady_clock::now();
    quad_build_total_us += chrono::duration_cast<chrono::microseconds>(toc - tic).count();
}

void CollisionManager::Collision(float dt)
{
    if (m_CollisionObjList.size() < 2)
    {
        Clear();
        return;
    }

    // 오브젝트 간 충돌 처리를 한다.
    list<Object*>::iterator iter;
    list<Object*>::iterator iterEnd = m_CollisionObjList.end();
    --iterEnd;
   
    for (iter = m_CollisionObjList.begin(); iter != iterEnd; ++iter)
    {
        list<Object*>::iterator jter = iter;
        ++jter;
        list<Object*>::iterator jterEnd = m_CollisionObjList.end();
        for (; jter != jterEnd; ++jter)
        {
            CheckCollision(*iter, *jter, dt);
        }
    }
    
    Clear();
}

void CollisionManager::Collision2(float dt)
{
    m_pCollTex->ClearBuffer(0, 0, GETRESOLUTION.x, GETRESOLUTION.y);

    const auto& collList = *(m_CollisionSpace->GetColliderList());
    int srcCollNum = (int)collList.size();

    if (srcCollNum < 2)
    {
        Clear();
        return;
    }

    auto tic = chrono::steady_clock::now();
    for (int i = 0; i < srcCollNum; ++i)
    {
        const auto& pSrc = collList[i];
        // 충돌체 검색
        vector<Collider*> pDstList;
        m_CollisionSpace->GetEqualSpaceColliders(pSrc, pDstList);
        vector<Collider*>::const_iterator iterDst;
        vector<Collider*>::const_iterator iterDstEnd = pDstList.end();
#ifdef _DEBUG
        if (pSrc->GetTag() == "PlayerBody")
        {
            DrawColliders(pDstList, nullptr);
        }
#endif
        int dstCollNum = (int)pDstList.size();
        for (int j = 0; j < dstCollNum; ++j)
        {
            const auto& pDst = pDstList[j];
#ifdef _DEBUG
            if (pDst->GetTag() == "PlayerBody")
            {
                DrawColliders({}, pSrc);
            }
#endif
            m_CollisionSpace->Mark(pSrc, pDst);

            // 충돌체가 서로 충돌했는가
            if (pSrc->CheckCollision(pDst))
            {
                // 히트 포인트 셋팅
                pDst->SetHitPoint(pSrc->GetHitPoint());

                // 충돌목록에서 이전에 충돌된 적이 없다면
                // 처음 막 충돌되었다는 의미
                if (!pSrc->CheckCollisionList(pDst))
                {
                    // 서로 상대방을 충돌 목록으로 추가한다.
                    pSrc->AddCollider(pDst);
                    pDst->AddCollider(pSrc);

                    pSrc->CallFunction(CS_ENTER, pDst, dt);
                    pDst->CallFunction(CS_ENTER, pSrc, dt);
                }
                // 기존 충돌된 적이 있다면 계속 충돌 상태인 것이다.
                else
                {
                    pSrc->CallFunction(CS_STAY, pDst, dt);
                    pDst->CallFunction(CS_STAY, pSrc, dt);
                }
            }
            // 현재 충돌하지 않았다면,
            // 현재 충돌이 안된 상태에서 이전에 충돌이 되고 있엇
            // 다면 이제 막 충돌상태에서 떨어졌다는 의미이다.
            else if (pSrc->CheckCollisionList(pDst))
            {
                // 서로 충돌이 안되므로 충돌목록에서 지워준다.
                pSrc->EraseCollisionList(pDst);
                pDst->EraseCollisionList(pSrc);

                pSrc->CallFunction(CS_LEAVE, pDst, dt);
                pDst->CallFunction(CS_LEAVE, pSrc, dt);
            }
        }
    }
    auto toc = chrono::steady_clock::now();
    collidecheck_total_us += chrono::duration_cast<chrono::microseconds>(toc - tic).count();
#ifdef  _DEBUG
    ++frame_count;
    if (frame_count == frame_num)
    {
        frame_count = 0;
        quad_build_total_us /= frame_num;
        collidecheck_total_us /= frame_num;
        _cprintf("QuadTree  build takes %f ms\n", (float)quad_build_total_us / 1000.f);
        _cprintf("Collision check takes %f ms\n", (float)collidecheck_total_us / 1000.f);
        quad_build_total_us = 0;
        collidecheck_total_us = 0;
    }
#endif //  _DEBUG
    Clear();
}

void CollisionManager::Draw(HDC hdc, float dt)
{
    m_pCollTex->DrawImageAt(hdc, Pos(0.f, 0.f));
}

bool CollisionManager::CheckCollision(Object* pSrc, Object* pDst, float dt)
{
    const list<Collider*>* pSrcList = pSrc->GetColliderList();
    const list<Collider*>* pDstList = pDst->GetColliderList();

    list<Collider*>::const_iterator iterSrc;
    list<Collider*>::const_iterator iterSrcEnd = pSrcList->end();

    list<Collider*>::const_iterator iterDst;
    list<Collider*>::const_iterator iterDstEnd = pDstList->end();

    bool bCollision = false;
    for (iterSrc = pSrcList->begin(); iterSrc != iterSrcEnd; ++iterSrc)
    {
        if (!(*iterSrc)->GetEnable())
        {
            continue;
        }
        for (iterDst = pDstList->begin(); iterDst != iterDstEnd; ++iterDst)
        {
            if (!(*iterDst)->GetEnable())
            {
                continue;
            }
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
            DrawColorRectWithOutBorder(m_pCollTex->GetDC(), screenRect, RGB(128, 0, 128));
        }
        if (pColl)
        {
            Rect screenRect = pColl->GetBounds().SubtractOffset(CAMERA->GetTopLeft());
            DrawColorRectWithOutBorder(m_pCollTex->GetDC(), screenRect, RGB(128, 0, 128));
        }
    }
}
