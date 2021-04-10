#include "CollisionSpace.h"
#include "Collider.h"
#include "../Core/Camera.h"
#include "../Scene/SceneManager.h"
#include "../Application/Window.h"
#include "../Resources/ResourceManager.h"
#include "../Core/Camera.h"
#include "../Object/Object.h"

unique_ptr<CollisionSpace> CollisionSpace::m_CurSpace = nullptr;

CollisionSpace::CollisionSpace()
{
}

void CollisionSpace::SetQuadTree()
{
	m_CurSpace = make_unique<CollisionSpace>();
	m_CurSpace->Init();
}

void CollisionSpace::Init()
{
	// 월드 좌표계 설정
	m_tWorldSpace = { 0.f, 0.f, 4096.f , 4096.f };

	// 헤드 초기화
	m_QuadHead = QuadSpace::MakeQuadPtr(0, 0, m_tWorldSpace);
	m_mapSpace.insert(make_pair(0, m_QuadHead.get()));

	// 콜라이더에 할당할 Id 큐 초기화
	for (int id = 0; id < m_iExpectedCollNum; ++id)
	{
		m_IdQueue.push(id);
	}

	// 충돌 체크 매트릭스 초기화
	m_CheckMat.resize(m_iExpectedCollNum);
	fill(m_CheckMat.begin(), m_CheckMat.end(), vector<bool>(m_iExpectedCollNum, false));

	// 충돌체 담는 컨테이너
	m_ColliderContainer.resize(m_iExpectedCollNum);
	fill(m_ColliderContainer.begin(), m_ColliderContainer.end(), nullptr);
}

CollisionSpace::~CollisionSpace()
{
	for (Collider*& pColl : m_ColliderContainer)
	{
		if (pColl)
		{
			pColl->SetId(-1);
			pColl->SetSpaceId(-1);
		}
	}
	m_ColliderContainer.clear();
}

void CollisionSpace::QuadSpace::Clear()
{
	for (int i = 0; i < 4; ++i)
	{
		if (m_QuadPartitions[i])
		{
			m_CurSpace->m_mapSpace.erase(m_QuadPartitions[i]->m_iIdx);
			m_QuadPartitions[i]->Clear();
			m_QuadPartitions[i] = nullptr;
		}
	}
}

void CollisionSpace::QuadSpace::GetChildCollidersNum(size_t* const sz)
{
	for (int i = 0; i < 4; ++i)
	{
		if (m_QuadPartitions[i])
		{
			*sz += m_QuadPartitions[i]->GetColliderNum();
			m_QuadPartitions[i]->GetChildCollidersNum(sz);
		}
	}
}

void CollisionSpace::Ready()
{
	// 체크 행렬 초기화
	auto curSize = m_CheckMat.size();
	fill(m_CheckMat.begin(), m_CheckMat.end(), vector<bool>(curSize, false));
}

void CollisionSpace::Update()
{
	m_tCameraSpace = CAMERA->GetWorldRect();
}

void CollisionSpace::Observe(Collider* pColl)
{
	// 쿼드 트리에 있던 콜라이더 일 경우
	if (pColl->GetId() != -1)
	{
		if (!pColl->IsMoved())
			return;

		ErasePreviousCollider(pColl);
	}
	// ID 큐에서 꺼냄
	int nxt_id = m_IdQueue.top();
	m_IdQueue.pop();

	// 전체 콜라이더 컨테이너에 할당
	++m_CurSize;
	pColl->SetId(nxt_id);
	m_ColliderContainer[nxt_id] = pColl;
	m_QuadHead->Insert(pColl);

	if (m_IdQueue.size() == 0)
	{
		ExpandId();
	}
}

void CollisionSpace::Mark(Collider* pSrc, Collider* pDst)
{
	int srcId = pSrc->GetId(), dstId = pDst->GetId();
	m_CheckMat[srcId][dstId] = m_CheckMat[dstId][srcId] = true;
}

void CollisionSpace::ErasePreviousCollider(Collider* pColl)
{
	int collId = pColl->GetId();
	if (collId == -1)
		return;

	// 전체 콜라이더 컨테이너에서 삭제
	--m_CurSize;
	m_ColliderContainer[collId] = nullptr;
	pColl->SetId(-1);

	// Id 큐에 추가
	m_IdQueue.push(collId);

	// 기존 쿼드 트리 공간에서 삭제
	int spaceId = pColl->GetSpaceId();
	const QuadParentPtr& quad = FindSpace(spaceId);
	list<Collider*>& collList = quad->m_CollList;
	list<Collider*>::iterator iterEnd = collList.end();
	for (auto iter = collList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter) == pColl)
		{
			pColl->SetSpaceId(-1);
			collList.erase(iter);
			break;
		}
	}

	// 필요시 공간 재조정 (O(V), for total space V)
	if (spaceId != 0)
	{
		int parentId = (spaceId - 1) / 4;
		QuadParentPtr parentQuad = FindSpace(parentId);
		
		// 반드시 있어야함
		assert(parentQuad);

		// 부모 공간에 있는 콜라이더
		auto& parentColliders = parentQuad->m_CollList;
		size_t totalSpaceCollNum = parentColliders.size();

		// 부모에 있는 콜라이더와 자식에 있는 콜라이더 수를 구한다 -> O(V), 비싼 계산은 아님
		parentQuad->GetChildCollidersNum(&totalSpaceCollNum);

		// 다시 합친다.
		if (totalSpaceCollNum <= m_iMaxObjectNum && parentQuad->m_QuadPartitions[0])
		{
			parentQuad->Merge(parentId, parentQuad->m_CollList);
		}
	}
}

void CollisionSpace::QuadSpace::Merge(int parentId, list<Collider*> &parentColliders)
{
	// 콜라이더 공간 id 재조정 및 부모 공간에 추가
	const auto adjust_collider = [&parentColliders, &parentId](Collider* const& pColl)
	{
		pColl->SetSpaceId(parentId);
		parentColliders.push_back(pColl);
	};

	auto& mapSpace = m_CurSpace->m_mapSpace;
	for (int i = 0; i < 4; ++i)
	{
		if (m_QuadPartitions[i])
		{
			const auto& childColliders = m_QuadPartitions[i]->m_CollList;
			for_each(childColliders.begin(), childColliders.end(), adjust_collider);

			// 서브 트리 합병
			m_QuadPartitions[i]->Merge(parentId, parentColliders);
			m_QuadPartitions[i] = nullptr;

			// 해쉬맵에서 지움
			int childId = m_iIdx * 4 + i + 1;
			mapSpace.erase(childId);
		}
	}
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

void CollisionSpace::GetEqualSpaceColliders(Collider* const &pSrc, vector<Collider*>& dstColliders)
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
	size_t totalSize = 0;
	m_QuadHead->GetChildCollidersNum(&totalSize);
	_cprintf("Managed Collider Num : %zd\n", totalSize);
#endif // _DEBUG
	m_QuadHead->Draw(hdc, dt);
}

void CollisionSpace::ExpandId()
{
	auto curSize = m_CheckMat.size();

	// 체크 매트릭스 확장
	m_CheckMat.resize(2 * curSize);
	fill(m_CheckMat.begin(), m_CheckMat.end(), vector<bool>(2 * curSize, false));

	// 충돌체 컨테이너 확장
	for (int i = 0; i < (int)curSize; ++i)
		m_ColliderContainer.push_back(nullptr);

	// 충돌체 ID 큐 확장
	for (int i = (int)curSize; i <(int) 2 * curSize; ++i)
		m_IdQueue.push(i);
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
		int idx = 4 * m_iIdx + i + 1;
		m_QuadPartitions[i] = MakeQuadPtr(m_iLevel + 1, idx, MakeArea(static_cast<Partition>(i)));
		m_CurSpace->m_mapSpace.insert(make_pair(idx, m_QuadPartitions[i].get()));
	}
}

void CollisionSpace::QuadSpace::Search(Collider* const& pSrc, vector<Collider*>& dstColliders)
{
	const int& srcId = pSrc->GetId();
	const auto& checkMat = m_CurSpace->m_CheckMat;

	// 같은 오브젝트내의 콜라이더가 아니면서 콜라이더 체크를 하지 않은 애들 
	const auto InsertByNotEqObjAndNotChecked = [&](Collider* const& pDst)
	{
		const int& dstId = pDst->GetId();
		bool checked = checkMat[srcId][dstId] || checkMat[dstId][srcId];
		if (pSrc->GetObj() != pDst->GetObj() && !checked)
		{
			dstColliders.push_back(pDst);
		}
	};

	for_each(m_CollList.begin(), m_CollList.end(), InsertByNotEqObjAndNotChecked);

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


CollisionSpace::QuadParentPtr CollisionSpace::FindSpace(int id)
{
	if (id == -1)
		return nullptr;

	auto found = m_mapSpace.find(id);
	if (found == m_mapSpace.end())
		return nullptr;
	return found->second;
}

bool CollisionSpace::QuadSpace::OutSideOfScreen(Collider* pColl)
{
	if (pColl->IsUICollider())
	{
		// UI인 경우 Screen 공간 상에서 계산
		const Rect& rect = pColl->GetBounds();
		if (rect.right < 0) return true;
		if (rect.left >= GETRESOLUTION.x) return true;
		if (rect.bottom < 0) return true;
		if (rect.top >= GETRESOLUTION.y) return true;
		return false;
	}

	// 일반 오브젝트는 World 공간 상에서 계산
	const Rect& rect = pColl->GetBounds();
	const Rect& camSpace = m_CurSpace->m_tCameraSpace;
	if (rect.right < camSpace.left) return true;
	if (rect.left >= camSpace.right) return true;
	if (rect.bottom < camSpace.top) return true;
	if (rect.top >= camSpace.bottom) return true;
	return false;
}

CollisionSpace::QuadPtr CollisionSpace::QuadSpace::MakeQuadPtr(unsigned int level, int idx, const Rect& rect)
{
	return make_unique<QuadSpace>(level, idx, rect);
}

CollisionSpace::QuadSpace::QuadSpace(unsigned int level, int idx, const Rect& rect)
	:
	m_iLevel(level),
	m_iIdx(idx),
	m_tArea(rect)
{
}
bool CollisionSpace::QuadSpace::IsOverLoaded() const
{
	return m_CollList.size() > m_iMaxObjectNum && m_QuadPartitions[0] == nullptr
		&& (m_tArea.right - m_tArea.left) / 2.f >= m_fMinSize;
}

void CollisionSpace::QuadSpace::AddCollider(Collider* const& pColl)
{
	pColl->SetSpaceId(m_iIdx);
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
	const auto& iterEnd = m_ColliderContainer.end();
	for (auto iter = m_ColliderContainer.begin(); iter != iterEnd; ++ iter)
	{
		if ((*iter) == nullptr) continue;
		if ((*iter)->GetTag() == strTag)
		{
			return (*iter);
		}
	}
	return nullptr;
}

