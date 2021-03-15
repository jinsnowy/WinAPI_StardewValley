#include "CollisionManager.h"
#include "../Object/Object.h"
#include "../Object/PointObject.h"
#include "../Collider/ColliderPoint.h"
#include "../Collider/ColliderRect.h"
#include "../Resources/ResourceManager.h"
#include "../Scene/SceneManager.h"
#include "../Core/Input.h"
#include "Collider.h"

DEFINITION_SINGLE(CollisionManager)

CollisionManager::CollisionManager()
{
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

void CollisionManager::AddCollideRect(const Pos& pos, const Rect& rect, const string& strTag)
{
    Object* pPoint = Object::CreateObject<PointObject>(strTag);
    pPoint->SetPos(pos.x, pos.y);

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

void CollisionManager::AddObject(Object* pObj)
{
    if (pObj->CheckCollider())
    {
        m_CollisionObjList.push_back(pObj);
    }
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
            COLL_CHANNEL eListen = (*iter)->GetListenChannel();
            if (eListen != CO_ALL && eListen != (*jter)->GetAdvertiseChannel())
            {
                continue;
            }
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
        for (iterDst = pDstList->begin(); iterDst != iterDstEnd; ++iterDst)
        {
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
