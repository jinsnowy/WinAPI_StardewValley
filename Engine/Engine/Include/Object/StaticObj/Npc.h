#pragma once
#include "StaticObject.h"
class Npc :
    public StaticObject
{
	friend class Object;
protected:
	Npc();
	Npc(const Npc& obj);
	virtual ~Npc();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual Npc* Clone();
private:
	shared_ptr<class UI> m_Store = nullptr;
	void Click(class Collider* pSrc, class Collider* pDst, float dt);
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual void LateInit();
};