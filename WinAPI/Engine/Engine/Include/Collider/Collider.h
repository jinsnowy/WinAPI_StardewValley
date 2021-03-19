#pragma once
#include "../Object/Ref.h"
#include "../framework.h"

class Collider : public Ref
{
	friend class Object;
protected:
	Collider();
	Collider(const Collider& coll);
	virtual ~Collider() = 0;
protected:
	bool			m_bUIColl;
	COLLIDER_TYPE	m_eCollType;
	class Object*	m_pObject;
	list<Collider*> m_CollisionList;
	list <function<void(Collider*, Collider*, float)>> m_FuncList[CS_END];
	Pos				m_tHitPoint;
public:
	void EnableUICollider(bool bUI) { m_bUIColl = bUI; }
	Pos GetHitPoint() const { return m_tHitPoint; }
	void SetHitPoint(const Pos& tPos) { m_tHitPoint = tPos; }
	void AddCollisionFunction(COLLISION_STATE eState, function<void(Collider*, Collider*, float)> &pFunc)
	{
		m_FuncList[eState].push_back(pFunc);
	}
	void AddCollisionFunction(COLLISION_STATE eState, void (*pFunc)(Collider*, Collider*, float))
	{
		m_FuncList[eState].push_back(pFunc);
	}
	template<typename T>
	void AddCollisionFunction(COLLISION_STATE eState, T* pObj, void (T::*pFunc)(Collider*, Collider*, float))
	{
		function<void(Collider*, Collider*, float)> func;
		func = bind(pFunc, pObj, placeholders::_1, placeholders::_2, placeholders::_3);
		m_FuncList[eState].push_back(func);
	}
	void CallFunction(COLLISION_STATE eState, Collider* pDst, float dt)
	{
		list<function<void(Collider*, Collider*, float)>>::iterator iter;
		list<function<void(Collider*, Collider*, float)>>::iterator iterEnd = m_FuncList[eState].end();

		for (iter = m_FuncList[eState].begin(); iter != iterEnd; ++iter)
		{
			(*iter)(this, pDst, dt);
		}
	}
public:
	void AddCollider(Collider* pCollider)
	{
		m_CollisionList.push_back(pCollider);
	}
	bool CheckCollisionList(Collider* pCollider)
	{
		list<Collider*>::iterator iter;
		list<Collider*>::iterator iterEnd = m_CollisionList.end();
		for (iter = m_CollisionList.begin(); iter != iterEnd; ++iter)
		{
			if (*iter == pCollider)
			{
				return true;
			}
		}
		return false;
	}
	void EraseCollisionList(Collider* pCollider)
	{
		list<Collider*>::iterator iter;
		list<Collider*>::iterator iterEnd = m_CollisionList.end();
		for (iter = m_CollisionList.begin(); iter != iterEnd; ++iter)
		{
			if (*iter == pCollider)
			{
				m_CollisionList.erase(iter);
				break;
			}
		}
	}
	COLLIDER_TYPE GetColliderType() const
	{
		return m_eCollType;
	}
	class Object* GetObj() const
	{
		return m_pObject;
	}
	void SetObj(class Object* pObject);
public:
	virtual bool Init() = 0;
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual bool CheckCollision(Collider* pDst);
	virtual void Draw(HDC hdc, float dt);
	virtual Collider* Clone() = 0;
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
protected:
	bool CollisionRectToRect(const Rect& src, const Rect& dst);
	bool CollisionRectToSphere(const Rect& src, const Sphere& dst);
	bool CollisionSphereToSphere(const Sphere& src, const Sphere& dst);
	bool CollisionRectToPixel(const Rect& src, const vector<Pixel>& vecPixel, int iWidth, int iHeight);
	bool CollisionSphereToPixel(const Sphere& src, const vector<Pixel>& vecPixel, int iWidth, int iHeight);
	bool CollisionRectToPoint(const Rect& src, const Pos& dst);
	bool CollisionSphereToPoint(const Sphere& src, const Pos& dst);
	bool CollisionPixelToPoint(const vector<Pixel>& vecPixel, int iWidth, int iHeight, const Pos& dst);
};

