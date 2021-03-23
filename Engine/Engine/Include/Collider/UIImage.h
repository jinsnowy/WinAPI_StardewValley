#pragma once
#include "UI.h"

class UIImage : public UI
{
private:
	friend class Object;
	friend class Scene;
protected:
	UIImage();
	UIImage(const UIImage ui);
	virtual ~UIImage() = 0;
public:
	virtual bool Init() = 0;
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual UIImage* Clone() = 0;
};

