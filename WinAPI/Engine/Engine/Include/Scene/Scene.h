#pragma once
#include "../framework.h"
#include "Layer.h"
#include "../Object/Object.h"

class Scene
{
	friend class Object;
	friend class Layer;
	friend class SceneManager;
	friend class PrototypeManager;
private:
	SCENE_CREATE m_eSceneType;
public:
	SCENE_CREATE GetSceneType() const { return m_eSceneType; }
	void SetSceneType(SCENE_CREATE eType) { m_eSceneType = eType; }
private:
	list <Object*> m_ObjList;
public:
	const list<Object*>* AccessObjList() const { return &m_ObjList; }
	Object* FindObjectByIndex(const INDEX& index) const;
	Object* FindObject(const string& tag);
	void AddObject(Object* pObj, Layer* pLayer = nullptr);
	void EraseObject(Object* pObj);
	void EraseObject(const string& tag);
	void EraseAllObjects();
	template<typename T>
	T* CreateObject(const string& strTag, Layer* pLayer = nullptr)
	{
		T* pObj = Object::CreateObject<T>(strTag);

		if (pLayer)
		{
			pLayer->AddObject(pObj);
		}

		AddObject(pObj);
		return pObj;
	}
protected:
	Scene();
	virtual ~Scene() = 0;
	list<Layer*> m_LayerList;
	Layer* CreateLayer(const string& tag, int zOrder = 0);
	static bool LayerSort(const Layer* pL1, const Layer* pL2);
public:
	Layer* FindLayer(const string& layerTag);
protected:
	virtual bool Init();
	virtual void Input(float dt);
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
protected:
	void LoadStage(const string& objectTag, const string& strlayerTag, FILE* pFile);
	virtual void LoadDefaultStages(const char* fileName);
	virtual void SaveDefaultStages(const vector<class Stage*>& saveStages, const char* fileName);
public:
	static string GetNearObjectName(Scene* pScene, const Pos& worldPos);
	static string ConvertToNameOption(TILE_OPTION eOpt);
};