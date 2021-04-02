#pragma once
#include "../framework.h"
class Collider;
class Object;
class CollisionSpace
{
public:
	CollisionSpace();
	~CollisionSpace();
private:
	class QuadSpace;
	typedef QuadSpace* QuadParentPtr;
	typedef unique_ptr<QuadSpace> QuadPtr;
	class QuadSpace
	{
		friend class CollisionSpace;
	private:
		enum class Partition
		{
			LEFT_TOP = 0,
			RIGHT_TOP = 1,
			LEFT_BOTTOM = 2,
			RIGHT_BOTTOM = 3,
			EQUAL_AREA = 4,
			OUT_SIDE = 5,
		};
	public:
		~QuadSpace();
		QuadSpace(const QuadSpace& space) = delete;
		QuadSpace(unsigned int level, size_t idx, const Rect& rect);
	private:
		unsigned int m_iLevel = 0;
		size_t m_iIdx = -1;
		Rect m_tArea = {};
		QuadParentPtr m_pParent;
		array<QuadPtr, 4> m_QuadPartitions = {};
		list<Collider*> m_CollList;
	private:
		static QuadPtr MakeQuadPtr(unsigned int level, size_t idx, const Rect& rect);
		static bool OutSideOfScreen(Collider* pColl);
		Partition GetPartition(const Rect& bounds);
		Rect MakeArea(Partition ePart) const;
		void SplitArea();
		void Search(class Collider* const& pSrc, vector<Collider*>& dstColliders);
	public:
		void Clear();
		bool Empty() const { return m_CollList.empty(); }
		bool IsOverLoaded() const;
		void AddCollider(Collider* const& pColl);
		void Insert(Collider* const& pColl);
		void Draw(HDC& hdc, const float& dt);
	};
private:
	friend class CollisionManager;
	using Partition = CollisionSpace::QuadSpace::Partition;
	using QuadSpace = CollisionSpace::QuadSpace;
	bool m_bCameraInit = false;
	static int m_iCollideNum;
	static constexpr size_t m_iMaxObjectNum = 4;
	static constexpr float m_fMinSize = 32.f;
	static constexpr int m_iExpectedCollNum = 20;
	static vector<vector<bool>> m_CheckMat;
	static QuadPtr m_QuadHead;
	static Rect m_tWorldSpace;
	static Rect m_tCameraSpace;
	vector<Collider*> m_Colliders;
private:
	void Init();
	void Clear();
	void Mark(Collider* pSrc, Collider* pDst);
	void GetEqualSpaceColliders(Collider* pSrc, vector<Collider*>& dstColliders);
	void ExpandCheckMat();
	void Draw(HDC hdc, float dt);
public:
	void Observe(Collider* pColl);
	const vector<Collider*>* GetColliderList() const { return &m_Colliders; }
	Collider* FindCollider(const string& strTag);
};