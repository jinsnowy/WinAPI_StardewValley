#pragma once
#include "../Object.h"
class Item : public Object
{
	friend class Object;
private:
	int m_iItemNum = 0;
public:
	void SetItemNum(int num) { m_iItemNum = num; }
	int GetItemNum() const { return m_iItemNum; }
	void Increase() { ++m_iItemNum; }
	void Decrease() {if(m_iItemNum > 0) --m_iItemNum; }
protected:
	Item();
	Item(const Item& item);
	virtual ~Item() = 0;
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual Item* Clone() = 0;
};