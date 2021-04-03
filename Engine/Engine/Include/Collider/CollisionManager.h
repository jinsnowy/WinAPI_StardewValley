#pragma once
#include "../framework.h"

class CollisionManager
{
DECLARE_SINGLE(CollisionManager)
private:
	class Texture* m_pCollTex = nullptr;
	list <class Object*> m_tempCollisionObjList;
	list <class Object*> m_CollisionObjList;
	class CollisionSpace* m_CollisionSpace = nullptr;
	void NaiveAdd(class Object* const& pObj);
public:
	bool Init();
	void ClickPoint();
	void AddCollideObject(class Object* pObj);
	void AddCollideRect(const Pos& pos, const Rect& rect, const string& strTag);
	void AddCollidePoint(const Pos& pos, const string& strTag);
	void AddObject(class Object* pObj);


	void SetUpCollisionSpace(SCENE_CREATE sc);
	void CollisionListVersion(float dt);
	void CollisionQuadTreeVersion(float dt);
	void Draw(HDC hdc, float dt);
	bool CheckCollision(class Object* pSrc, class Object* pDst, float dt);
	void DrawColliders(const vector<class Collider*> &colls, class Collider* pColl);
	void Clear();
};