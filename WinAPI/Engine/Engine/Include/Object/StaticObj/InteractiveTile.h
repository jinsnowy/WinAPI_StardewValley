#pragma once
#include "Tile.h"
#include "../Item/Item.h"

class InteractiveTile :
    public Tile
{
	friend class Object;
	friend class Stage;
private:
	float m_iHP = 0.f;
	class Item* m_pItem = nullptr;
protected:
	InteractiveTile();
	InteractiveTile(const InteractiveTile& obj);
	virtual ~InteractiveTile();
	virtual void TileHit(Collider* pSrc, Collider* pDst, float dt) = 0;
public:
	void SetHP(float hp) { m_iHP = hp; }
	void GetDamage(float power) { m_iHP -= power; }
	bool IsDie() const { return m_iHP <= 0.f; }
	void SetDropItem(class Item* pItem);
	virtual void ItemDrop(int num);
	virtual void Die() = 0;
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual InteractiveTile* Clone() = 0;
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};