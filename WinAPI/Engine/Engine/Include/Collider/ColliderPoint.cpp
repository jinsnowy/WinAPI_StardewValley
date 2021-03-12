#include "ColliderPoint.h"
#include "ColliderRect.h"
#include "ColliderSphere.h"
#include "ColliderPixel.h"
#include "../Object/Object.h"
#include "../Core/Camera.h"

ColliderPoint::ColliderPoint()
	: m_tPos(0,0), m_tWorldPos(0,0)
{
	m_eCollType = CT_POINT;
}

ColliderPoint::ColliderPoint(const ColliderPoint& coll)
	: Collider(coll)
{
	m_tPos = coll.m_tPos;
	m_tWorldPos = coll.m_tWorldPos;
}

ColliderPoint::~ColliderPoint()
{
}

bool ColliderPoint::Init()
{
	return true;
}

void ColliderPoint::Input(float dt)
{
	Collider::Input(dt);
}

int ColliderPoint::Update(float dt)
{
	Collider::Update(dt);
	return 0;
}

int ColliderPoint::LateUpdate(float dt)
{
	Collider::LateUpdate(dt);
	Pos tPos = m_pObject->GetPos();
	m_tWorldPos.x = tPos.x + m_tPos.x;
	m_tWorldPos.y = tPos.y + m_tPos.y;
	return 0;
}

bool ColliderPoint::CheckCollision(Collider* pDst)
{
	switch (pDst->GetColliderType())
	{
	case CT_RECT:
		return CollisionRectToPoint(static_cast<ColliderRect*>(pDst)->GetWorldInfo(), m_tWorldPos);
	case CT_SPHERE:
		return CollisionSphereToPoint(static_cast<ColliderSphere*>(pDst)->GetWorldInfo(), m_tWorldPos);
	case CT_PIXEL:
		return CollisionPixelToPoint(static_cast<ColliderPixel*>(pDst)->GetPixel(),
									static_cast<ColliderPixel*>(pDst)->GetWidth(),
									static_cast<ColliderPixel*>(pDst)->GetHeight(), m_tWorldPos);
	}
	return false;
}

void ColliderPoint::Draw(HDC hdc, float dt)
{
	Collider::Draw(hdc, dt);

	HPEN myPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	HPEN OldPen = (HPEN)SelectObject(hdc, myPen);

	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Pos tPos = Pos(0, 0);

	if (!m_bUIColl)
		tPos = CAMERA->GetTopLeft();

	int left = int(m_tWorldPos.x) - 3;
	int right = int(m_tWorldPos.x) + 3;
	int top = int(m_tWorldPos.y) - 3;
	int bottom = int(m_tWorldPos.y) + 3;

	Ellipse(hdc, left, top, right, bottom);

	DeleteObject(SelectObject(hdc, OldPen));
	DeleteObject(SelectObject(hdc, OldBrush));
}

ColliderPoint* ColliderPoint::Clone()
{
	return new ColliderPoint(*this);
}

void ColliderPoint::Save(FILE* pFile)
{
	Collider::Save(pFile);

}

void ColliderPoint::Load(FILE* pFile)
{
	Collider::Load(pFile);
}
