#pragma once
#include "Collider.h"
class ColliderPoint : public Collider
{
	friend class Object;
protected:
	ColliderPoint();
	ColliderPoint(const ColliderPoint& coll);
	virtual ~ColliderPoint();
protected:
	Pos m_tPos;
	Pos m_tWorldPos;
public:
	Pos GetInfo() const
	{
		return m_tPos;
	}
	Pos GetWorldInfo() const 
	{
		return m_tWorldPos;
	}
	void SetPoint(const Pos& pos)
	{
		m_tPos = pos;
	}
	void SetPoint(float x, float y)
	{
		m_tPos.x = x;
		m_tPos.y = y;
	}
	void SetDistance(const Pos& tDist)
	{
		m_tPos = tDist;
	}
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual bool CheckCollision(Collider* pDst);
	virtual void Draw(HDC hdc, float dt);
	virtual ColliderPoint* Clone();
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

