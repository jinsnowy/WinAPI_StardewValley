#include "CollisionSpace.h"
#include "Collider.h"
#include "../Core/Camera.h"
#include "../Scene/SceneManager.h"
#include "../Application/Window.h"
#include "../Resources/ResourceManager.h"

int CollisionSpace::m_iCollideNum = 0;
CollisionSpace::QuadPtr CollisionSpace::m_QuadHead = nullptr;
unordered_map<size_t, CollisionSpace::QuadParentPtr> CollisionSpace::m_mapQuadParent;

CollisionSpace::CollisionSpace()
{
}

CollisionSpace::~CollisionSpace()
{
}

list<Object*> CollisionSpace::GetEqualSpaceColliders(Object* pObj)
{
	return list<Object*>();
}

void CollisionSpace::QuadSpace::Clear()
{
	m_iCollideNum = 0;
	m_CollList.clear();
	for (int i = 0; i < 4; ++i)
	{
		if (m_QuadParitions[i])
		{
			m_QuadParitions[i]->Clear();
		}
		m_QuadParitions[i] = nullptr;
	}
}

CollisionSpace* CollisionSpace::MakeCollisionSpace()
{
	CollisionSpace* pSpace = new CollisionSpace;
	pSpace->m_tScreenSpace = {0.f, 0.f, float(GETRESOLUTION.x), float(GETRESOLUTION.y)};
	return pSpace;
}

void CollisionSpace::Observe(Collider* pColl)
{
	if (QuadSpace::OutSideOfScreen(pColl))
		return;

	if (!m_QuadHead)
	{
		m_QuadHead = QuadSpace::MakeQuadPtr(0, 0, m_tScreenSpace);
		m_mapQuadParent.insert(make_pair(0, m_QuadHead.get()));
	}

	++m_iCollideNum;
	m_QuadHead->Insert(pColl);
}

void CollisionSpace::QuadSpace::Insert(Collider* const& pColl)
{
	// 이미 분할된 공간이 있는 경우
	if (m_QuadParitions[(int)Partition::LEFT_TOP])
	{
		// 공간이 작은 경우 같은 공간에 넣음
		bool tooSmall = m_tArea.right - m_tArea.left < m_fMinSize;
		Partition ePart = tooSmall ? Partition::EQUAL_AREA : GetPartition(pColl);

		// 분할 공간으로 재귀
		if (ePart != Partition::EQUAL_AREA)
		{
			m_QuadParitions[(int)ePart]->Insert(pColl);
			return;
		}
	}

	// 충돌체 추가
	AddCollider(pColl);

	// 너무 많은 충돌체가 같이 있다면 공간 분할
	if (IsOverLoaded())
	{
		SplitArea();

		// 충돌체를 서브 영역으로 삽입
		for (auto iter = m_CollList.begin(); iter != m_CollList.end();)
		{
			Partition ePart = GetPartition((*iter));

			if (ePart != Partition::EQUAL_AREA)
			{
				m_QuadParitions[(int)ePart]->Insert((*iter));
				iter = m_CollList.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}


void CollisionSpace::QuadSpace::Draw(HDC& hdc, const float& dt)
{
	// Pos tPoint = m_tPivot - CAMERA->GetTopLeft();
		//DrawPointWithColor(hdc, tPoint, util::Blue);
	DrawVoidRectWithBorderColor(hdc, m_tArea, util::Green);
	for (int i = 0; i < 4; ++i)
	{
		if (m_QuadParitions[i])
		{
			m_QuadParitions[i]->Draw(hdc, dt);
		}
	}
}

void CollisionSpace::Draw(HDC hdc, float dt)
{
#ifdef _DEBUG
	_cprintf("Managed Collider Num :%d\n", m_iCollideNum);
#endif // _DEBUG
	if (m_QuadHead)
	{
		m_QuadHead->Draw(hdc, dt);
	}
}

void CollisionSpace::Clear()
{
	for (int i = 0; i < 4; ++i)
	{
		m_QuadHead->Clear();
	}
	m_QuadHead = nullptr;
	m_mapQuadParent.clear();
}

CollisionSpace::QuadSpace::~QuadSpace()
{
}

Rect CollisionSpace::QuadSpace::MakeArea(Partition ePart) const
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

void CollisionSpace::QuadSpace::SplitArea()
{
	for (int i = 0; i < 4; ++i)
	{
		size_t idx = 4 * m_iIdx + i;
		m_QuadParitions[i] = MakeQuadPtr(m_iLevel + 1, idx, MakeArea(static_cast<Partition>(i)));
		m_mapQuadParent.insert(make_pair(idx, m_QuadParitions[i].get()));
	}
}

bool CollisionSpace::QuadSpace::OutSideOfScreen(Collider* pColl)
{
	if (pColl->IsUICollider())
		return false;

	Rect rect = pColl->GetBounds();
	rect = rect.SubtractOffset(CAMERA->GetTopLeft());
	if (rect.right < 0) return true;
	if (rect.left >= GETRESOLUTION.x) return true;
	if (rect.bottom < 0) return true;
	if (rect.top >= GETRESOLUTION.y) return true;
	return false;
}

shared_ptr<CollisionSpace::QuadSpace> CollisionSpace::QuadSpace::MakeQuadPtr(unsigned int level, size_t idx, const Rect& rect)
{
	return make_shared<QuadSpace>(level, idx, rect);
}

CollisionSpace::QuadSpace::QuadSpace(unsigned int level, size_t idx, const Rect& rect)
	:
	m_iLevel(level),
	m_iIdx(idx),
	m_tArea(rect),
	m_pParent(this)
{

}

bool CollisionSpace::QuadSpace::IsOverLoaded() const
{
	return m_CollList.size() > m_iMaxObjectNum && m_QuadParitions[0] == nullptr
		&& (m_tArea.right - m_tArea.left) / 2.f >= m_fMinSize;
}

void CollisionSpace::QuadSpace::AddCollider(Collider* const& pColl)
{
	m_CollList.push_back(pColl);
	pColl->SetSpaceId(m_iIdx);
}

CollisionSpace::QuadSpace::Partition CollisionSpace::QuadSpace
::GetPartition(Collider* pColl)
{
	Rect bounds = pColl->GetBounds().SubtractOffset(CAMERA->GetTopLeft());
	float CenterX = (m_tArea.left + m_tArea.right) / 2.f;
	float CenterY = (m_tArea.top + m_tArea.bottom) / 2.f;

	// Left
	if (bounds.right < CenterX)
	{
		// Top
		if (bounds.bottom < CenterY)
		{
			return Partition::LEFT_TOP;
		}
		// Bottom
		else if(bounds.top > CenterY)
		{
			return Partition::LEFT_BOTTOM;
		}
	}
	// Right
	else if(bounds.left > CenterX)
	{
		// Top
		if (bounds.bottom < CenterY)
		{
			return Partition::RIGHT_TOP;
		}
		// Bottom
		else if(bounds.top > CenterY)
		{
			return Partition::RIGHT_BOTTOM;
		}
	}

	return Partition::EQUAL_AREA;
}



