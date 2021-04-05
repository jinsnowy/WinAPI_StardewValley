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
		QuadSpace(unsigned int level, int idx, const Rect& rect);
	private:
		unsigned int m_iLevel = 0;
		int m_iIdx = -1;
		Rect m_tArea = {};
		array<QuadPtr, 4> m_QuadPartitions = {};
		list<Collider*> m_CollList;
	private:
		static QuadPtr MakeQuadPtr(unsigned int level, int idx, const Rect& rect);
		static bool OutSideOfScreen(Collider* pColl);
		size_t GetColliderNum() const { return m_CollList.size(); }
		Partition GetPartition(const Rect& bounds);
		Rect MakeArea(Partition ePart) const;
		void SplitArea();
		void Search(class Collider* const& pSrc, vector<Collider*>& dstColliders);
		void Merge(int parentId, list<Collider*>& parentColliders);
	public:
		void Clear();
		void GetChildCollidersNum(size_t* const sz);
		bool Empty() const { return m_CollList.empty(); }
		bool IsOverLoaded() const;
		void AddCollider(Collider* const& pColl);
		void Insert(Collider* const& pColl);
		void Draw(HDC& hdc, const float& dt);
	};
private:
	friend class CollisionManager;
	friend class QuadSpace;
	using Partition = CollisionSpace::QuadSpace::Partition;
	using QuadSpace = CollisionSpace::QuadSpace;
	static constexpr size_t m_iMaxObjectNum = 4;
	static constexpr float m_fMinSize = 32.f;
	static constexpr int m_iExpectedCollNum = 20;
	static unique_ptr<CollisionSpace> m_CurSpace;
	int m_CurSize = 0;
	QuadPtr m_QuadHead = nullptr;
	Rect m_tWorldSpace = {};
	Rect m_tCameraSpace = {};
	vector<vector<bool>> m_CheckMat = {};
	unordered_map<int, QuadParentPtr> m_mapSpace = {};
	priority_queue<int, vector<int>, greater<int>> m_IdQueue = {};
	vector<Collider*> m_ColliderContainer = {};
private:
	void Init();
	void Mark(Collider* pSrc, Collider* pDst);
	void GetEqualSpaceColliders(Collider* const&  pSrc, vector<Collider*>& dstColliders);
	void ExpandId();
	QuadParentPtr FindSpace(int id);
	void Draw(HDC hdc, float dt);
public:
	void Ready();
	void Update();
	int GetCurSize() const { return m_CurSize; }
	static CollisionSpace* GetCurQuadTree() { return m_CurSpace.get(); }
	static void SetQuadTree();
	void ErasePreviousCollider(Collider* pColl);
	void Observe(Collider* pColl);
	const vector<Collider*>* GetColliderList() const { return &m_ColliderContainer; }
	Collider* FindCollider(const string& strTag);
};