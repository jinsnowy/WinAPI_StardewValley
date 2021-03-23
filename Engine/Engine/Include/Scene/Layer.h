#pragma once
#include "../framework.h"

class Scene;
class Object;
class Layer
{
private:
	friend class Scene;
	friend class PrototypeManager;
	friend class MapEditScene;
	Layer();
private:
	bool		m_bEnable;
	bool		m_bLife;
	int			m_zOrder;
	string	m_tag;
	Scene* m_pScene;
	list<Object*> m_LayerObjList;
public:
	const list<Object*>* GetObjList() const { return &m_LayerObjList; }
	void EraseObject(Object* pObj);
	void AddObject(Object* pObj);
	void ClearObject();
public:
	~Layer();
	// Get/Set method
	bool GetEnable() const { return m_bEnable; }
	bool GetLife() const { return m_bLife; }

	void Die() { m_bLife = false; }
	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	int  GetZOrder() const { return m_zOrder; }
	string GetTag() const { return m_tag; }
	class Scene* GetScene() const { return m_pScene; }
	void SetZOrder(int zOrder) { m_zOrder = zOrder; }
	void SetTag(const string& tag) { m_tag = tag; }
	void SetScene(class Scene* pScene) { m_pScene = pScene; }
	static bool ObjectYAxisSort(const Object* pObjA, const Object* pObjB);
public:
	void Input(float dt);
	void Update(float dt);
	void LateUpdate(float dt);
	void Collision(float dt);
	void Draw(HDC hdc, float dt);
};

