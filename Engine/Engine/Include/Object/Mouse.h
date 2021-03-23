#pragma once
#include "StaticObj/UI.h"
class Mouse : public UI
{
private:
	friend class Object;
	friend class Scene;
protected:
	Mouse();
	Mouse(const Mouse& ui);
	virtual ~Mouse();
private:
	Pos m_tMove;
	Pos m_tWorldPos;
public:
	Pos GetMove() const { return m_tMove; }
	Pos GetWorldPos() const { return m_tWorldPos; }

public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual Mouse* Clone();
};

