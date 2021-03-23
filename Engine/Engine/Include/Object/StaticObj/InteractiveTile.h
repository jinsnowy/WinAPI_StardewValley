#pragma once
#include "Tile.h"
#include "../Item/Tool.h"
#include "../Item/Item.h"

class InteractiveTile :
    public Tile
{
	friend class Object;
	friend class Stage;
private:
	float m_iHP = 100.f;
	class Item* m_pItem = nullptr;
protected:
	InteractiveTile();
	InteractiveTile(const InteractiveTile& obj);
	virtual ~InteractiveTile();
	virtual bool DieCondition() const { return false; }
	virtual void TileHit(Collider* pSrc, Collider* pDst, float dt);
	void GetDamageFromPlayerTool();
public:
	void SetHP(float hp) { m_iHP = hp; }
	void GetDamage(float power) { m_iHP -= power; }
	void CheckDie();
	void SetDropItem(class Item* pItem);
	virtual void ItemDrop(int num, bool effect = true);
	virtual void AfterDie();
	void SetPosByIndex(const INDEX& index)
	{
		float sz = (TILESIZE - GetSize().x) / 2;
		SetPos(float(index.x * TILESIZE + sz), float(index.y * TILESIZE + sz));
	}
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual InteractiveTile* Clone() = 0;
	virtual void LateInit() = 0;
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};