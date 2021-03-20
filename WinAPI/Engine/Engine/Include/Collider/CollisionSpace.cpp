#include "CollisionSpace.h"
#include "../Core/Camera.h"

int CollisionSpace::m_iPointNum = 0;

CollisionSpace::CollisionSpace()
{
}

CollisionSpace::~CollisionSpace()
{
	Clear();
}

list<Object*> CollisionSpace::GetEqualSpaceColliders(Object* pObj)
{
	return list<Object*>();
}

CollisionSpace* CollisionSpace::MakeCollisionSpace(const Rect& tWorldArea)
{
	CollisionSpace* pSpace = new CollisionSpace;
	pSpace->m_tWorldSpace = tWorldArea;
	return pSpace;
}

void CollisionSpace::AddPoint(const Pos& pos)
{
	if (!m_QuadHead)
	{
		++m_iPointNum;
		m_QuadHead = new QuadSpace(0, m_tWorldSpace, pos);
		return;
	}
	m_QuadHead->AddPoint(pos);
}

void CollisionSpace::QuadSpace::AddPoint(const Pos& point)
{
	CollisionSpace::QuadSpace::Partition ePart = GetPartition(point);

	// 월드 세계 바깥 좌표
	if (ePart == CollisionSpace::QuadSpace::Partition::OUT_SIDE)
		return;

	QuadSpace* &pQuad = m_QuadParitions[(int)ePart];
	if (pQuad)
	{
		return pQuad->AddPoint(point);
	}
	// 이 공간이 비어있는 경우
	++m_iPointNum;
	size_t id = 4 * m_iIndex + (size_t)(ePart);
	pQuad = new QuadSpace(id, BuildArea(ePart), point);
}

void CollisionSpace::QuadSpace::Draw(HDC& hdc, const float& dt)
{
	Pos tPoint = m_tPoint - CAMERA->GetTopLeft();
	Rect toCamera = m_tArea.SubtractOffset(CAMERA->GetTopLeft());
	DrawVoidRectWithBorderColor(hdc, toCamera, util::Green);
	DrawPointWithColor(hdc, tPoint, util::Blue);

	for (int i = 0; i < 4; ++i)
	{
		if (m_QuadParitions[i])
		{
			m_QuadParitions[i]->Draw(hdc, dt);
		}
	}
}

void CollisionSpace::Clear()
{
	SAFE_DELETE(m_QuadHead);
	m_iPointNum = 0;
}

void CollisionSpace::Draw(HDC hdc, float dt)
{
	// _cprintf("%d num points added\n", m_iPointNum);
	if (m_QuadHead)
	{
		m_QuadHead->Draw(hdc, dt);
	}
}

CollisionSpace::QuadSpace::~QuadSpace()
{
	for (int i = 0; i < 4; ++i)
	{
		if (m_QuadParitions[i])
		{
			SAFE_DELETE(m_QuadParitions[i]);
		}
	}
}

CollisionSpace::QuadSpace::QuadSpace(const size_t& index, const Rect& rect, const Pos& pivot)
	:
	m_iIndex(index),
	m_tArea(rect),
	m_tPoint(pivot)
{
}

Rect CollisionSpace::QuadSpace::BuildArea(Partition ePart) const
{
	float CenterX = (m_tArea.left + m_tArea.right) / 2.f;
	float CenterY = (m_tArea.top + m_tArea.bottom) / 2.f;

	Rect area = {};
	switch (ePart)
	{
	case CollisionSpace::QuadSpace::Partition::LEFT_TOP:
		area.left = m_tArea.left;
		area.top = m_tArea.top;
		area.right = CenterX;
		area.bottom = CenterY;
		break;
	case CollisionSpace::QuadSpace::Partition::LEFT_BOTTOM:
		area.left = m_tArea.left;
		area.top = CenterY;
		area.right = CenterX;
		area.bottom = m_tArea.bottom;
		break;
	case CollisionSpace::QuadSpace::Partition::RIGHT_TOP:
		area.left = CenterX;
		area.top = m_tArea.top;
		area.right = m_tArea.right;
		area.bottom = CenterY;
		break;
	case CollisionSpace::QuadSpace::Partition::RIGHT_BOTTOM:
		area.left = CenterX;
		area.top = CenterY;
		area.right = m_tArea.right;
		area.bottom = m_tArea.bottom;
		break;
	}
	return area;
}

CollisionSpace::QuadSpace::Partition CollisionSpace::QuadSpace::GetPartition(const Pos& point)
{
	float CenterX = (m_tArea.left + m_tArea.right) / 2.f;
	float CenterY = (m_tArea.top + m_tArea.bottom) / 2.f;

	// 사각형 너무 작은 경우
	if (CenterX - m_tArea.left < m_fMinSize)
		return  CollisionSpace::QuadSpace::Partition::OUT_SIDE;

	float px = point.x;
	float py = point.y;

	// leftTop
	if (px >= m_tArea.left && px < CenterX)
	{
		if (py >= m_tArea.top && py < CenterY)
		{
			return CollisionSpace::QuadSpace::Partition::LEFT_TOP;
		}
		else
		{
			return CollisionSpace::QuadSpace::Partition::LEFT_BOTTOM;
		}
	}
	else {
		if (py >= m_tArea.top && py < CenterY)
		{
			return CollisionSpace::QuadSpace::Partition::RIGHT_TOP;
		}
		else {
			return CollisionSpace::QuadSpace::Partition::RIGHT_BOTTOM;
		}
	}

	return CollisionSpace::QuadSpace::Partition::OUT_SIDE;
}

void CollisionSpace::QuadSpace::AddObject(Object* pObj)
{
}

