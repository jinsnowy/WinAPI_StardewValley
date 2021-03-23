#pragma once
#include "UI.h"
class UIPanel : public UI
{
private:
	friend class Object;
private:
	UIPanel();
	UIPanel(const UIPanel& ui);
	virtual ~UIPanel();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual UIPanel* Clone();
};

