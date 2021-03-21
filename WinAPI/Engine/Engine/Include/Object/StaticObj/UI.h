#pragma once
#include "../Object.h"
class UI : public Object
{
private:
	friend class Object;
public:
	UI();
	UI(const UI& ui);
	virtual ~UI() = 0;
public:
	virtual bool Init() = 0;
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual UI* Clone() = 0;
};

