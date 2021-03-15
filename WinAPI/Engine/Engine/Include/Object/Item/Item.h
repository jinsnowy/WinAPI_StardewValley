#pragma once
#include "../Object.h"

class Item : public Object
{
	friend class Object;
private:
	static constexpr float m_fDropAngle = DEG2RAD(75.f);
	static constexpr float m_fDropAngleVar = DEG2RAD(10.f);
	static constexpr float m_fDropVelo = 300.f;
	static constexpr float m_fDropVeloVar = DEG2RAD(50.f);
	static normal_distribution<float> m_AngleDist;
	static normal_distribution<float> m_VeloDist;
	static unordered_map<string, Item*> m_mapItem;
public:
	static Item* FindItem(const string& strItemKey);
	static Item* CreateCloneItem(const string& strItemKey,
							const wchar_t* pFileName,
							const string& strPathKey = TEXTURE_PATH);
	static void ClearItemMap()
	{
		Safe_Release_Map(m_mapItem);
	}
protected:
	bool m_bToolItem = false;
	int m_iItemNum = 1;
	static constexpr float m_fChaseSpeed = 300.f;
	static constexpr float m_fChaseRange = 300.f;
private:
	class BoundEffect* m_Effect = nullptr;
	void ChasePlayer(float dt);
public:
	void GenerateBoundEffect();
	bool IsToolItem() const { return m_bToolItem; }
	void SetItemNum(int num) { m_iItemNum = num; }
	int GetItemNum() const { return m_iItemNum; }
	void Increase() { ++m_iItemNum; }
	void Decrease() {if(m_iItemNum > 0) --m_iItemNum; }
protected:
	Item();
	Item(const Item& item);
	virtual ~Item();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual Item* Clone();
};