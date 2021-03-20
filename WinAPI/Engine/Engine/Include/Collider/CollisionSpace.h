#pragma once
#include "../framework.h"
class Collider;
class Object;
class CollisionSpace
{
	friend class GameScene;
private:
	CollisionSpace();
	~CollisionSpace();
private:
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
			OUT_SIDE = 4,
		};
	public:
		~QuadSpace();
		QuadSpace(const QuadSpace& space) = delete;
		explicit QuadSpace(const size_t& index, const Rect& rect, const Pos& pivot);
	private:
		size_t m_iIndex = -1;
		Rect m_tArea = {};
		Pos m_tPoint = Pos();
		std::array<QuadSpace*, 4> m_QuadParitions = {};
		list<Object*> m_eqObjList;
	private:
		Rect BuildArea(Partition ePart) const;
	public:
		Partition GetPartition(const Pos& point);
		void AddObject(Object* pObj);
		void AddPoint(const Pos& point);
		void Draw(HDC& hdc, const float& dt);
	};
private:
	static int m_iPointNum;
	static constexpr float m_fMinSize = 1.0f;
	QuadSpace* m_QuadHead = nullptr;
	Rect m_tWorldSpace = {};
public:
	static CollisionSpace* MakeCollisionSpace(const Rect& tWorldSpace);
	void AddPoint(const Pos& pos);
	list<Object*> GetEqualSpaceColliders(Object* pObj);
public:
	void Clear();
	void Draw(HDC hdc, float dt);
};