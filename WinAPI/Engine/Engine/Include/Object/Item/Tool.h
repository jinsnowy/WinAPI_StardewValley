#pragma once
#include "Item.h"
class Tool : public Item
{
	friend class Object;
private:
	int m_iPower = 0;
	int m_iEndure = 0;
public:
	int GetPower() const { return m_iPower; }
	int GetEndure() const { return m_iEndure; }
	void SetPower(int power) { m_iPower = power; }
	void SetEndure(int endure) { m_iEndure = endure; }
protected:
	Tool();
	Tool(const Tool& item);
	virtual ~Tool();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual Tool* Clone();
};

