#pragma once
#include "UI.h"
class UIScrollBar :
    public UI
{
private:
	friend class Object;
private:
	UIScrollBar();
	UIScrollBar(const UIScrollBar& ui);
	virtual ~UIScrollBar();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual UIScrollBar* Clone();
};
