#pragma once
#include "../framework.h"

class CollisionManager
{
DECLARE_SINGLE(CollisionManager)
private:
	list <class Object*> m_tempCollisionObjList;
	list <class Object*> m_CollisionObjList;
public:
	bool Init();
	void ClickPoint();
	void AddCollidePoint(const Pos& pos, const string& strTag);
	void AddCollideAttackPoint(const Pos& pos, const string& strTag, float power);
	void TempLateUpdate(float dt);


	void AddObject(class Object* pObj);
	void Collision(float dt);
	bool CheckCollision(class Object* pSrc, class Object* pDst, float dt);
	void Clear();
};