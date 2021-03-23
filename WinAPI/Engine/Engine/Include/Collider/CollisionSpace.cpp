#include "CollisionSpace.h"
#include "Collider.h"
#include "../Core/Camera.h"
#include "../Scene/SceneManager.h"
#include "../Application/Window.h"
#include "../Resources/ResourceManager.h"

int CollisionSpace::m_iCollideNum = 0;
CollisionSpace::QuadPtr CollisionSpace::m_QuadHead = nullptr;
unordered_map<size_t, CollisionSpace::QuadParentPtr> CollisionSpace::m_mapQuadParent;
vector<vector<bool>> CollisionSpace::m_CheckMat;

CollisionSpace::CollisionSpace()
{
	m_tScreenSpace = { 0.f, 0.f, float(GETRESOLUTION.x), float(GETRESOLUTION.y) };
	m_CheckMat.resize(m_iExpectedCollNum);
	fill(m_CheckMat.begin(), m_CheckMat.end(), vector<bool>(m_iExpectedCollNum, false));
}

CollisionSpace::~CollisionSpace()
{
}

void CollisionSpace::QuadSpace::Clear()
{
	m_iCollideNum = 0;
	m_CollList.clear();
	for (int i = 0; i < 4; ++i)
	{
		if (m_QuadPartitions[i])
		{
			m_QuadPartitions[i]->Clear();
		}
		if (m_iLevel + 1 > m_iMinLevel)
		{
			m_QuadPartitions[i] = nullptr;
		}
	}
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

	pColl->SetId(m_iCollideNum);
	++m_iCollideNum;

	m_Colliders.push_back(pColl);
	m_QuadHead->Insert(pColl, QuadSpace::GetScreenRect(pColl));

	if (m_iCollideNum > (int) m_CheckMat.size())
	{
		ExpandCheckMat();
	}
}

void CollisionSpace::Mark(Collider* pSrc, Collider* pDst)
{
	int srcId = pSrc->GetId(), dstId = pDst->GetId();
	m_CheckMat[srcId][dstId] = m_CheckMat[dstId][srcId] = true;
}

void CollisionSpace::QuadSpace::Insert(Collider* const& pColl, const Rect& bounds)
{
	// 이미 분할된 공간이 있는 경우
	if (m_QuadPartitions[(int)Partition::LEFT_TOP])
	{
		// 공간이 작은 경우 같은 공간에 넣음
		bool tooSmall = m_tArea.right - m_tArea.left < m_fMinSize;
		Partition ePart = tooSmall ? Partition::EQUAL_AREA : GetPartition(bounds);

		// 분할 공간으로 재귀
		if (ePart != Partition::EQUAL_AREA)
		{
			m_QuadPartitions[(int)ePart]->Insert(pColl, bounds);
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
			Rect screenRect = GetScreenRect((*iter));
			Partition ePart = GetPartition(screenRect);

			if (ePart != Partition::EQUAL_AREA)
			{
				m_QuadPartitions[(int)ePart]->Insert((*iter), screenRect);
				iter = m_CollList.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}

void CollisionSpace::GetEqualSpaceColliders(Collider* pSrc, vector<Collider*>& dstColliders)
{
	if (m_QuadHead)
	{
		m_QuadHead->Search(pSrc, QuadSpace::GetScreenRect(pSrc), dstColliders);
	}
}


void CollisionSpace::QuadSpace::Draw(HDC& hdc, const float& dt)
{
	DrawVoidRectWithBorderColor(hdc, m_tArea, util::Green);

	for (int i = 0; i < 4; ++i)
	{
		if (m_QuadPartitions[i])
		{
			m_QuadPartitions[i]->Draw(hdc, dt);
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
	m_mapQuadParent.clear();

	// 충돌 체크 정보 초기화
	m_Colliders.clear();
	m_CheckMat.resize(m_iExpectedCollNum);
	fill(m_CheckMat.begin(), m_CheckMat.end(), vector<bool>(m_iExpectedCollNum, false));
}

void CollisionSpace::ExpandCheckMat()
{
	auto curSize = m_CheckMat.size();
	m_CheckMat.resize(2 * curSize, vector<bool>(2 * curSize, false));
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
		m_QuadPartitions[i] = MakeQuadPtr(m_iLevel + 1, idx, MakeArea(static_cast<Partition>(i)));
		m_mapQuadParent.insert(make_pair(idx, m_QuadPartitions[i].get()));
	}
}

void CollisionSpace::QuadSpace::Search(Collider* const& pSrc, const Rect& bounds, vector<Collider*>& dstColliders)
{
	int srcId = pSrc->GetId();
	const auto InsertNotEqObjAndNotChecked = [&, srcId](Collider* pDst)
	{
		int dstId = pDst->GetId();
		bool checked = m_CheckMat[srcId][dstId] || m_CheckMat[dstId][srcId];
		if (pSrc->GetObj() != pDst->GetObj() && !checked)
		{
			dstColliders.push_back(pDst);
		}
	};

	for_each(m_CollList.begin(), m_CollList.end(), InsertNotEqObjAndNotChecked);

	if (m_QuadPartitions[0])
	{
		Partition ePart = GetPartition(bounds);

		if (ePart == Partition::EQUAL_AREA)
		{
			for (int i = 0; i < 4; ++i)
			{
				if (m_QuadPartitions[i]->m_tArea.IsCollideRect(bounds))
				{
					m_QuadPartitions[i]->Search(pSrc, bounds, dstColliders);
				}
			}
		}
		else 
		{
			m_QuadPartitions[(int)ePart]->Search(pSrc, bounds, dstColliders);
		}
	}
}

bool CollisionSpace::QuadSpace::OutSideOfScreen(Collider* pColl)
{
	if (pColl->IsUICollider())
		return false;

	Rect rect = GetScreenRect(pColl);
	if (rect.right < 0) return true;
	if (rect.left >= GETRESOLUTION.x) return true;
	if (rect.bottom < 0) return true;
	if (rect.top >= GETRESOLUTION.y) return true;
	return false;
}

Rect CollisionSpace::QuadSpace::GetScreenRect(Collider* pColl)
{
	if (pColl->IsUICollider())
	{
		return pColl->GetBounds();
	}
	return pColl->GetBounds().SubtractOffset(CAMERA->GetTopLeft());
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
	return m_CollList.size() > m_iMaxObjectNum && m_QuadPartitions[0] == nullptr
		&& (m_tArea.right - m_tArea.left) / 2.f >= m_fMinSize;
}

void CollisionSpace::QuadSpace::AddCollider(Collider* const& pColl)
{
	m_CollList.push_back(pColl);
}

CollisionSpace::QuadSpace::Partition CollisionSpace::QuadSpace
::GetPartition(const Rect& bounds)
{
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



