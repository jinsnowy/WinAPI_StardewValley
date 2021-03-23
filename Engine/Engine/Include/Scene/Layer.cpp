#include "Layer.h"
#include "Scene.h"
#include "../Object/Object.h"
#include "../Collider/CollisionManager.h"

Layer::Layer()
    :
    m_bEnable(true),
    m_bLife(true),
    m_zOrder(0),
    m_tag(""),
    m_pScene(nullptr)
{

}
Layer::~Layer()
{
    ClearObject();
}

void Layer::ClearObject()
{
    list<Object*>::iterator it;
    list<Object*>::iterator iterEnd = m_LayerObjList.end();

    for (it = m_LayerObjList.begin(); it != iterEnd; it++)
    {
        m_pScene->EraseObject(*it);
        SAFE_RELEASE((*it));
    }
    m_LayerObjList.clear();
}

void Layer::EraseObject(Object* pObj)
{
    list<Object*>::iterator iter;
    list<Object*>::iterator iterEnd = m_LayerObjList.end();

    for (iter = m_LayerObjList.begin(); iter != iterEnd; ++iter)
    {
        if ((*iter) == pObj)
        {
            SAFE_RELEASE((*iter));
            m_pScene->EraseObject((*iter));
            m_LayerObjList.erase(iter);
            return;
        }
    }
}

void Layer::AddObject(Object* pObj)
{
    pObj->SetScene(m_pScene);
    pObj->SetLayer(this);
    pObj->AddRef();
    
    m_LayerObjList.push_back(pObj);
}

bool Layer::ObjectYAxisSort(const Object* pObjA, const Object* pObjB)
{
    return pObjA->GetPos().y < pObjB->GetPos().y;
}

void Layer::Input(float dt)
{
    auto iterEnd = m_LayerObjList.end();
    for (auto it = m_LayerObjList.begin(); it != iterEnd;)
    {
        if (!(*it)->GetEnable())
        {
            ++it;
            continue;
        }

        (*it)->Input(dt);

        if (!(*it)->GetLife())
        {
            m_pScene->EraseObject(*it);
            SAFE_RELEASE((*it));
            it = m_LayerObjList.erase(it);
            iterEnd = m_LayerObjList.end();
        }
        else ++it;
    }
}

void Layer::Update(float dt)
{
    auto iterEnd = m_LayerObjList.end();
    for (auto it = m_LayerObjList.begin(); it != iterEnd;)
    {
        if (!(*it)->GetEnable())
        {
            ++it;
            continue;
        }

        (*it)->Update(dt);

        if (!(*it)->GetLife())
        {
            m_pScene->EraseObject(*it);
            SAFE_RELEASE((*it));
            it = m_LayerObjList.erase(it);
            iterEnd = m_LayerObjList.end();
        }
        else ++it;
    }
}

void Layer::LateUpdate(float dt)
{
    auto iterEnd = m_LayerObjList.end();
    for (auto it = m_LayerObjList.begin(); it != iterEnd;)
    {
        if (!(*it)->GetEnable())
        {
            ++it;
            continue;
        }

        (*it)->LateUpdate(dt);

        if (!(*it)->GetLife())
        {
            m_pScene->EraseObject(*it);
            SAFE_RELEASE((*it));
            it = m_LayerObjList.erase(it);
            iterEnd = m_LayerObjList.end();
        }
        else ++it;
    }
}

void Layer::Collision(float dt)
{
    auto iterEnd = m_LayerObjList.end();
    for (auto it = m_LayerObjList.begin(); it != iterEnd;)
    {
        if (!(*it)->GetEnable())
        {
            ++it;
            continue;
        }

        if (!(*it)->GetLife())
        {
            m_pScene->EraseObject(*it);
            SAFE_RELEASE((*it));
            it = m_LayerObjList.erase(it);
            iterEnd = m_LayerObjList.end();
        }
        else 
        {
            COLLISION_MANAGER->AddObject(*it);
            ++it; 
        }
    }
}

void Layer::Draw(HDC hdc, float dt)
{
    if (GetTag() == "Object")
    {
        m_LayerObjList.sort(ObjectYAxisSort);
    }
    auto iterEnd = m_LayerObjList.end();
    for (auto it = m_LayerObjList.begin(); it != iterEnd;)
    {
        if (!(*it)->GetEnable())
        {
            ++it;
            continue;
        }

        (*it)->Draw(hdc, dt);

        if (!(*it)->GetLife())
        {
            m_pScene->EraseObject(*it);
            SAFE_RELEASE((*it));
            it = m_LayerObjList.erase(it);
            iterEnd = m_LayerObjList.end();
        }
        else ++it;
    }
}
