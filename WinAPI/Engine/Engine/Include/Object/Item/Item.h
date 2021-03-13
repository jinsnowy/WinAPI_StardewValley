#pragma once
#include "../Object.h"
class Item : public Object
{
	friend class Object;
	static unordered_map<string, Item*> m_mapItem;
public:
	static Item* FindItem(const string& strItemKey);
	static Item* LoadItem(const string& strItemKey,
		const wchar_t* pFileName,
		const string& strPathKey = TEXTURE_PATH);
	static void ClearItemMap()
	{
		Safe_Release_Map(m_mapItem);
	}
private:
	int m_iItemNum = 1;
public:
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