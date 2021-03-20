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


ColliderRect* CollisionManager::m_tCamWorldColl = nullptr;
ColliderRect* CollisionManager::m_tCamScreenColl = nullptr;

DEFINITION_SINGLE(CollisionManager)

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
    SAFE_RELEASE(m_tCamScreenColl);
    SAFE_RELEASE(m_tCamWorldColl);
}

bool CollisionManager::Init()
{
    m_tCamWorldColl = Object::CreateObject<ColliderRect>("CameraWorld");
    m_tCamScreenColl = Object::CreateObject<ColliderRect>("CameraScreen");
    return true;
}

void CollisionManager::ClickPoint()
{
    AddCollidePoint(MOUSEWORLDPOS, "Click");
}

void CollisionManager::AddCollideObject(Object* pObj)
{
    pObj->SetColliderChannel(CO_PLAYER);
    m_tempCollisionObjList.push_back(pObj);
    m_CollisionObjList.push_back(pObj);
}

void CollisionManager::AddCollideRect(const Pos& pos, const Rect& rect, const string& strTag)
{
    Object* pPoint = Object::CreateObject<PointObject>(strTag);
    pPoint->SetPos(pos.x, pos.y);
    pPoint->SetColliderChannel(CO_PLAYER);

    ColliderRect* pColl = pPoint->AddCollider<ColliderRect>(strTag);
    pColl->SetRect(rect.left, rect.top, rect.right, rect.bottom);
    SAFE_RELEASE(pColl);

    m_tempCollisionObjList.push_back(pPoint);
    m_CollisionObjList.push_back(pPoint);
}

void CollisionManager::AddCollidePoint(const Pos& pos, const string& strTag)
{
    Object* pPoint = Object::CreateObject<PointObject>(strTag);
    pPoint->SetPos(pos.x, pos.y);
    pPoint->SetColliderChannel(CO_PLAYER);

    ColliderPoint* pColl = pPoint->AddCollider<ColliderPoint>(strTag);
    SAFE_RELEASE(pColl);

    m_tempCollisionObjList.push_back(pPoint);
    m_CollisionObjList.push_back(pPoint);
}

void CollisionManager::TempLateUpdate(float dt)
{
    for (auto iter = m_tempCollisionObjList.begin();
        iter != m_tempCollisionObjList.end();
        ++iter)
    {
        (*iter)->LateUpdate(dt);
    }
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
}

void CollisionManager::NaiveAdd(Object* const& pObj)
{
    m_CollisionObjList.push_back(pObj);
}

void CollisionManager::ColliderCheckAdd(Object* const& pObj)
{
    bool isInsideObject = false;
    bool isInside = false;
    const list<Collider*>* collList = pObj->GetColliderList();
    for (auto iter = collList->begin(); iter != collList->end(); ++iter)
    {
        if ((*iter)->GetColliderType() == CT_PIXEL)
        {
            (*iter)->SetEnable(true);
            continue;
        }
        if ((*iter)->IsUICollider())
        {
            m_CollisionObjList.push_back(pObj);
            return;
        }
        else
        {
            isInside = m_tCamWorldColl->CheckCollision(*iter);
            (*iter)->SetEnable(isInside);
            isInsideObject = true;
        }
    }

    if (isInsideObject)
    {
        m_CollisionObjList.push_back(pObj);
    }
}

void CollisionManager::DrawCullingAdd(Object* pObj)
{
    if (pObj->IsUIObject())
    {
        m_CollisionObjList.push_back(pObj);
        return;
    }

    const Size& tSize = pObj->GetSize();
    Pos tPos = pObj->GetPos() - tSize * pObj->GetPivot();
    tPos -= CAMERA->GetTopLeft();

    // 카메라 컬링
    const RESOLUTION& tClientRS = CAMERA->GetClientRS();
    if (tPos.x + tSize.x < 0 || tPos.x > tClientRS.x || tPos.y + tSize.y < 0 || tPos.y > tClientRS.y)
    {
        return;
    }
    m_CollisionObjList.push_back(pObj);
}
void CollisionManager::AddObject(Object* pObj)
{
    if (pObj->CheckCollider())
    {
        NaiveAdd(pObj);
        // DrawCullingAdd(pObj);
        Scene* pScene = pObj->GetScene();
        if (pScene)
        {
            bool isGameScene = (pScene->GetSceneType() != SC_START && pScene->GetSceneType() != SC_MAPEDIT);
            if (isGameScene)
            {
                static_cast<GameScene*>(pScene)->AddQuadSpacePoint(pObj->GetTopLeft());
            }
        }
        // ColliderCheckAdd(pObj);
        // 
    }

}

void CollisionManager::EraseObject(Object* pObj)
{
    for (auto iter = m_CollisionObjList.begin();
        iter != m_CollisionObjList.end();
        ++iter)
    {
        if ((*iter) == pObj)
        {
            m_CollisionObjList.erase(iter);
            return;
        }
    }
}

void CollisionManager::Update(float dt)
{
    const RESOLUTION& tClientRS = CAMERA->GetClientRS();
    const Pos& topLeft = CAMERA->GetTopLeft();

    m_tCamWorldColl->SetRect(topLeft.x, topLeft.y, topLeft.x + tClientRS.x, topLeft.y + tClientRS.y);
    m_tCamScreenColl->SetRect(0, 0, tClientRS.x, tClientRS.y);
}

void CollisionManager::Collision(float dt)
{
    if (m_CollisionObjList.size() < 2)
    {
        Clear();
        return;
    }

    // 임시 콜라이더 위치 업데이트
    TempLateUpdate(dt);

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
