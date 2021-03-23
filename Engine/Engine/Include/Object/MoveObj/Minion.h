#pragma once
#include "MovableObject.h"
class Minion : public MovableObject
{
	friend class Object;
private:
	MOVE_DIR	m_Dir;
	float		m_FireTime;
	float		m_FireLimitTime;
private:
	Minion();
	Minion(const Minion& obj);
	~Minion();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual Minion* Clone();
public:
	void CollisionBullet(class Collider* pSrc, class Collider* pDst, float dt);
private:
	void Fire();
};

