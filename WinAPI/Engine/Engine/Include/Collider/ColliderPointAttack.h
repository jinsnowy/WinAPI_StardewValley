#pragma once
#include "ColliderPoint.h"
class ColliderPointAttack :
    public ColliderPoint
{
	friend class Object;
private:
	float m_fPower;
protected:
	ColliderPointAttack();
	ColliderPointAttack(const ColliderPointAttack& coll);
	virtual ~ColliderPointAttack();
public:
	void SetPower(float power) { m_fPower = power; }
	float GetPower() const { return m_fPower; }
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual bool CheckCollision(Collider* pDst);
	virtual void Draw(HDC hdc, float dt);
	virtual ColliderPointAttack* Clone();
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

