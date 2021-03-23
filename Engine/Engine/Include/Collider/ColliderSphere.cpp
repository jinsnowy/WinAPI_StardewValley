#include "ColliderSphere.h"
#include "ColliderRect.h"
#include "ColliderPixel.h"
#include "ColliderPoint.h"
#include "../Object/Object.h"
#include "../Core/Camera.h"

ColliderSphere::ColliderSphere()
{
    m_eCollType = CT_SPHERE;
}

ColliderSphere::ColliderSphere(const ColliderSphere& coll)
    :
    Collider(coll)
{
    m_tInfo = coll.m_tInfo;
    m_tWorldInfo = coll.m_tWorldInfo;
}

ColliderSphere::~ColliderSphere()
{
}

bool ColliderSphere::Init()
{
    return true;
}

void ColliderSphere::Input(float dt)
{
    Collider::Input(dt);
}

int ColliderSphere::Update(float dt)
{
    Collider::Update(dt);
    return 0;
}

int ColliderSphere::LateUpdate(float dt)
{
    Collider::LateUpdate(dt);

    Pos tPos = m_pObject->GetPos();
    m_tWorldInfo.center = tPos + m_tInfo.center;
    m_tWorldInfo.radius = m_tInfo.radius;

    return 0;
}

bool ColliderSphere::CheckCollision(Collider* pDst)
{
    switch (pDst->GetColliderType())
    {
    case CT_SPHERE:
        return CollisionSphereToSphere(m_tWorldInfo, static_cast<ColliderSphere*>(pDst)->GetWorldInfo());
    case CT_RECT:
        return CollisionRectToSphere(static_cast<ColliderRect*>(pDst)->GetWorldInfo(), m_tWorldInfo);
    case CT_POINT:
        return CollisionSphereToPoint(m_tWorldInfo, static_cast<ColliderPoint*>(pDst)->GetWorldInfo());
    }
    return false;
}

void ColliderSphere::Draw(HDC hdc, float dt)
{
    Collider::Draw(hdc,dt);

#ifdef _DEBUG
    HPEN myPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
    HPEN OldPen = (HPEN)SelectObject(hdc, myPen);

    HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Pos tPos = Pos(0, 0);

    if (!m_bUIColl)
        tPos = CAMERA->GetTopLeft();

    int left = m_tWorldInfo.center.x - m_tWorldInfo.radius - tPos.x;
    int right = left + 2 * m_tWorldInfo.radius;
    int top = m_tWorldInfo.center.y - m_tWorldInfo.radius - tPos.y;
    int bottom = top + 2 * m_tWorldInfo.radius;

    Ellipse(hdc, left, top, right, bottom);

    DeleteObject(SelectObject(hdc, OldPen));
    DeleteObject(SelectObject(hdc, OldBrush));
#endif
}

ColliderSphere* ColliderSphere::Clone()
{
    return new ColliderSphere(*this);
}

void ColliderSphere::Save(FILE* pFile)
{
    Collider::Save(pFile);
}

void ColliderSphere::Load(FILE* pFile)
{
    Collider::Load(pFile);
}
