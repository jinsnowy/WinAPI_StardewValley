#pragma once
#include "../framework.h"

class CollisionManager
{
DECLARE_SINGLE(CollisionManager)
private:
	class Texture* m_pCollTex = nullptr;
	list <class Object*> m_tempCollisionObjList;
	list <class Object*> m_CollisionObjList;
	unique_ptr<class CollisionSpace> m_CollisionSpace;
	void NaiveAdd(class Object* const& pObj);
	void DrawCullingAdd(class Object* pObj);
public:
	bool Init();
	void ClickPoint();
	void AddCollideObject(class Object* pObj);
	void AddCollideRect(const Pos& pos, const Rect& rect, const string& strTag);
	void AddCollidePoint(const Pos& pos, const string& strTag);
	void AddObject(class Object* pObj);

	void Collision(float dt);
	void Collision2(float dt);
	void Draw(HDC hdc, float dt);
	bool CheckCollision(class Object* pSrc, class Object* pDst, float dt);
	void DrawColliders(const vector<class Collider*> &colls, class Collider* pColl);
	void Clear();
};