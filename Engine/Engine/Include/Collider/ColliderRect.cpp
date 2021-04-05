#include "ColliderRect.h"
#include "ColliderSphere.h"
#include "ColliderPixel.h"
#include "ColliderPoint.h"
#include "../Object/Object.h"
#include "../Core/Camera.h"

ColliderRect::ColliderRect()
	:
	Collider(),
	m_tInfo(),
	m_tWorldInfo()
{
}

ColliderRect::ColliderRect(const ColliderRect& coll)
	:
	Collider(coll),
	m_tInfo(coll.m_tInfo),
	m_tWorldInfo(coll.m_tWorldInfo)
{
}

ColliderRect::~ColliderRect()
{
}

void ColliderRect::SetRect(float l, float t, float r, float b)
{
	m_tInfo.SetRect(l, t, r, b);
}


bool ColliderRect::Init()
{
	return true;
}

void ColliderRect::Input(float dt)
{
	Collider::Input(dt);
}

int ColliderRect::Update(float dt)
{
	Collider::Update(dt);
	return 0;
}

int ColliderRect::LateUpdate(float dt)
{
	Collider::LateUpdate(dt);
	const Pos& tPos = m_pObject->GetPos();
	m_tWorldInfo.left = tPos.x + m_tInfo.left;
	m_tWorldInfo.top = tPos.y + m_tInfo.top;
	m_tWorldInfo.right = tPos.x + m_tInfo.right;
	m_tWorldInfo.bottom = tPos.y + m_tInfo.bottom;

	return 0;
}

bool ColliderRect::CheckCollision(Collider* pDst)
{
	switch (pDst->GetColliderType())
	{
	case CT_RECT:
		return CollisionRectToRect(m_tWorldInfo, static_cast<ColliderRect*>(pDst)->GetWorldInfo());
	case CT_SPHERE:
		return CollisionRectToSphere(m_tWorldInfo, static_cast<ColliderSphere*>(pDst)->GetWorldInfo());
	case CT_PIXEL:
		return CollisionRectToPixel(m_tWorldInfo, static_cast<ColliderPixel*>(pDst)->GetPixel(),
											      static_cast<ColliderPixel*>(pDst)->GetWidth(), 
											      static_cast<ColliderPixel*>(pDst)->GetHeight());
	case CT_POINT:
		return CollisionRectToPoint(m_tWorldInfo, static_cast<ColliderPoint*>(pDst)->GetWorldInfo());
	}
	return false;
}

void ColliderRect::Draw(HDC hDC, float dt)
{

	Collider::Draw(hDC, dt);

	Pos tPos = Pos(0, 0);
	if (!m_bUIColl)
		tPos = CAMERA->GetTopLeft();

	RECT rc;
	rc.top = int(m_tWorldInfo.top - tPos.y);
	rc.left = int(m_tWorldInfo.left - tPos.x);
	rc.bottom = int(m_tWorldInfo.bottom - tPos.y);
	rc.right = int(m_tWorldInfo.right - tPos.x);

	DrawRedRect(hDC, rc);
}

void ColliderRect::Save(FILE* pFile)
{
	Collider::Save(pFile);

	fwrite(&m_tInfo, sizeof(Rect), 1, pFile);
}

void ColliderRect::Load(FILE* pFile)
{
	Collider::Load(pFile);

	fread(&m_tInfo, sizeof(Rect), 1, pFile);
}

ColliderRect* ColliderRect::Clone()
{
	return new ColliderRect(*this);
}
