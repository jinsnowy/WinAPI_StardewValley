#pragma once
#include "../framework.h"
class Collider;
class Object;
class CollisionSpace
{
	friend class CollisionManager;
private:
	CollisionSpace();
	~CollisionSpace();
private:
	class QuadSpace;
	typedef QuadSpace* QuadParentPtr;
	typedef shared_ptr<QuadSpace> QuadPtr;
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
		array<QuadPtr, 4> m_QuadParitions = {};
		list<Collider*> m_CollList;
	private:
		static QuadPtr MakeQuadPtr(unsigned int level, size_t idx, const Rect& rect);
		static bool OutSideOfScreen(Collider* pColl);
		Partition GetPartition(Collider* pColl);
		Rect MakeArea(Partition ePart) const;
		void SplitArea();
	public:
		void Clear();
		bool Empty() const { return m_CollList.empty(); }
		bool IsOverLoaded() const;
		void AddCollider(Collider* const& pColl);
		void Insert(Collider* const& pColl);
		void Draw(HDC& hdc, const float& dt);
	};
private:
	using Partition = CollisionSpace::QuadSpace::Partition;
	using QuadSpace = CollisionSpace::QuadSpace;
	static int m_iCollideNum;
	static constexpr size_t m_iMaxObjectNum = 5;
	static constexpr float m_fMinSize = 25.f;
	static unordered_map<size_t, QuadParentPtr> m_mapQuadParent;
	static QuadPtr m_QuadHead;
	Rect m_tScreenSpace = {};
private:
	void Clear();
	bool ExistSpace(size_t id) { return m_mapQuadParent.find(id) != m_mapQuadParent.end(); }
public:
	static CollisionSpace* MakeCollisionSpace();
	void Observe(Collider* pColl);
	list<Object*> GetEqualSpaceColliders(Object* pObj);
public:
	void Draw(HDC hdc, float dt);

};