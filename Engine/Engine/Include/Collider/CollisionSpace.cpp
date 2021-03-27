#include "CollisionSpace.h"
#include "Collider.h"
#include "../Core/Camera.h"
#include "../Scene/SceneManager.h"
#include "../Application/Window.h"
#include "../Resources/ResourceManager.h"
#include "../Core/Camera.h"

int CollisionSpace::m_iCollideNum = 0;
CollisionSpace::QuadPtr CollisionSpace::m_QuadHead = nullptr;
vector<vector<bool>> CollisionSpace::m_CheckMat = {};
Rect CollisionSpace::m_tWorldSpace = {};
Rect CollisionSpace::m_tCameraSpace = {};

CollisionSpace::CollisionSpace()
{
}

void CollisionSpace::Init()
{
	Clear();

	m_tWorldSpace = { 0.f, 0.f, 4096.f , 4096.f };
	m_QuadHead = QuadSpace::MakeQuadPtr(0, 0, m_tWorldSpace);
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
	}
}

void CollisionSpace::Observe(Collider* pColl)
{
	if (!m_bCameraInit)
	{
		m_tCameraSpace = CAMERA->GetWorldRect();
		m_bCameraInit = true;
	}

	if (QuadSpace::OutSideOfScreen(pColl))
		return;

	pColl->SetId(m_iCollideNum);
	++m_iCollideNum;

	m_Colliders.push_back(pColl);
	m_QuadHead->Insert(pColl);

	if (m_iCollideNum >= (int) m_CheckMat.size())
	{
		ExpandCheckMat();
	}
}

void CollisionSpace::Mark(Collider* pSrc, Collider* pDst)
{
	int srcId = pSrc->GetId(), dstId = pDst->GetId();
	m_CheckMat[srcId][dstId] = m_CheckMat[dstId][srcId] = true;
}

void CollisionSpace::QuadSpace::Insert(Collider* const& pColl)
{
	// 이미 분할된 공간이 있는 경우
	if (m_QuadPartitions[(int)Partition::LEFT_TOP])
	{
		// 공간이 작은 경우 같은 공간에 넣음
		const Rect& bounds = pColl->GetBounds();
		bool tooSmall = m_tArea.right - m_tArea.left < m_fMinSize;
		Partition ePart = tooSmall ? Partition::EQUAL_AREA : GetPartition(bounds);

		// 분할 공간으로 재귀
		if (ePart != Partition::EQUAL_AREA)
		{
			m_QuadPartitions[(int)ePart]->Insert(pColl);
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
		const auto& iterEnd = m_CollList.end();
		for (auto iter = m_CollList.begin(); iter != iterEnd;)
		{
			const Rect& bounds = (*iter)->GetBounds();
			Partition ePart = GetPartition(bounds);

			if (ePart != Partition::EQUAL_AREA)
			{
				m_QuadPartitions[(int)ePart]->Insert((*iter));
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
	m_QuadHead->Search(pSrc, dstColliders);
}


void CollisionSpace::QuadSpace::Draw(HDC& hdc, const float& dt)
{
	Rect screenRect = m_tArea.SubtractOffset(CAMERA->GetTopLeft());
	DrawVoidRectWithBorderColor(hdc, screenRect, util::Green);

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
	m_bCameraInit = false;

	if (m_QuadHead)
	{
		for (int i = 0; i < 4; ++i)
		{
			m_QuadHead->Clear();
		}
	}

	// 충돌 체크 정보 초기화
	m_Colliders.clear();
	m_CheckMat.resize(m_iExpectedCollNum);
	fill(m_CheckMat.begin(), m_CheckMat.end(), vector<bool>(m_iExpectedCollNum, false));
}

void CollisionSpace::ExpandCheckMat()
{
	auto curSize = m_CheckMat.size();
	m_CheckMat.resize(2 * curSize);
	fill(m_CheckMat.begin(), m_CheckMat.end(), vector<bool>(2 * curSize, false));
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
	}
}

void CollisionSpace::QuadSpace::Search(Collider* const& pSrc, vector<Collider*>& dstColliders)
{
	const int& srcId = pSrc->GetId();
	const auto InsertNotEqObjAndNotChecked = [&](Collider* pDst)
	{
		const int& dstId = pDst->GetId();
		bool checked = m_CheckMat[srcId][dstId] || m_CheckMat[dstId][srcId];
		if (pSrc->GetObj() != pDst->GetObj() && !checked)
		{
			dstColliders.push_back(pDst);
		}
	};

	for_each(m_CollList.begin(), m_CollList.end(), InsertNotEqObjAndNotChecked);

	// 파티션 분리가 되어 있을 때
	if (m_QuadPartitions[0])
	{
		const Rect& bounds = pSrc->GetBounds();
		for (int i = 0; i < 4; ++i)
		{
			if (m_QuadPartitions[i]->m_tArea.IsCollideRect(bounds))
			{
				m_QuadPartitions[i]->Search(pSrc, dstColliders);
			}
		}
	}
}

bool CollisionSpace::QuadSpace::OutSideOfScreen(Collider* pColl)
{
	if (pColl->IsUICollider())
		return false;

	const Rect& rect = pColl->GetBounds();
	if (rect.right < m_tCameraSpace.left) return true;
	if (rect.left >= m_tCameraSpace.right) return true;
	if (rect.bottom < m_tCameraSpace.top) return true;
	if (rect.top >= m_tCameraSpace.bottom) return true;
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

Collider* CollisionSpace::FindCollider(const string& strTag)
{
	const auto& iterEnd = m_Colliders.end();
	for (auto iter = m_Colliders.begin(); iter != iterEnd; ++ iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			return (*iter);
		}
	}
	return nullptr;
}

