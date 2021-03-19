#pragma once
#include "Object.h"
class TempObject :
	public Object
{
	friend class Object;
private:
	TempObject();
	virtual ~TempObject();
	TempObject(TempObject& obj);
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual TempObject* Clone();
private:
	float m_fLifeDuration = 0.f;
public:
	void SetLifeDuration(float duration) { m_fLifeDuration = m_fLifeDuration; }
	float GetLifeDuration() const { return m_fLifeDuration; }
};
