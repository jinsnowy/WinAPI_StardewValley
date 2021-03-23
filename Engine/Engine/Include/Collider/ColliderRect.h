#pragma once
#include "Collider.h"

class ColliderRect : public Collider
{
	friend class Object;
protected:
	ColliderRect();
	ColliderRect(const ColliderRect& coll);
	virtual ~ColliderRect();
private:
	Rect m_tInfo;
	Rect m_tWorldInfo;
public:
	void SetRect(float l, float t, float r, float b);
	Rect GetInfo() const
	{
		return m_tInfo;
	}
	Rect GetWorldInfo() const 
	{
		return m_tWorldInfo;
	}
	Rect GetBounds() const
	{
		return m_tWorldInfo;
	}
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual bool CheckCollision(Collider* pDst);
	virtual void Draw(HDC hdc, float dt);
	virtual ColliderRect* Clone();
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};
