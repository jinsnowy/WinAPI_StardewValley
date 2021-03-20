#pragma once
#include "../framework.h"

class CollisionManager
{
DECLARE_SINGLE(CollisionManager)
private:
	list <class Object*> m_tempCollisionObjList;
	list <class Object*> m_CollisionObjList;
	static class ColliderRect *m_tCamWorldColl;
	static class ColliderRect *m_tCamScreenColl;
	void NaiveAdd(class Object* const& pObj);
	void ColliderCheckAdd(class Object* const& pObj);
	void DrawCullingAdd(class Object* pObj);
public:
	bool Init();
	void ClickPoint();
	void AddCollideObject(class Object* pObj);
	void AddCollideRect(const Pos& pos, const Rect& rect, const string& strTag);
	void AddCollidePoint(const Pos& pos, const string& strTag);
	void TempLateUpdate(float dt);
	void AddObject(class Object* pObj);
	void EraseObject(class Object* pObj);

	void Update(float dt);
	void Collision(float dt);
	bool CheckCollision(class Object* pSrc, class Object* pDst, float dt);
	void Clear();
};