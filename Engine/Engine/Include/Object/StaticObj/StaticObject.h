#pragma once
#include "../Object.h"

class StaticObject : public Object
{
protected:
	StaticObject();
	StaticObject(const StaticObject& obj);
	virtual ~StaticObject();
public:
	virtual bool Init() = 0;
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual StaticObject* Clone() = 0;
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual void LateInit();
};