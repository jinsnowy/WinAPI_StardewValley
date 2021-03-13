#pragma once
#include "../framework.h"
#include "Layer.h"
class Layer;
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
	list <class Object*> m_ObjList;
	unordered_map<string, class Object*> m_mapProtoType;
	void EraseAllObjects();
protected:
	const list<class Object* >* GetObjList() const { return &m_ObjList; }
	Object* FindObject(const string& tag);
public:
	void AddObject(Object* pObj, class Layer* pLayer = nullptr);
	void EraseObject(Object* pObj);
	void EraseObject(const string& tag);
	template<typename T>
	T* CreateObject(const string& strTag, class Layer* pLayer = nullptr)
	{
		T* pObj = new T;
		pObj->SetTag(strTag);
		pObj->SetScene(this);

		if (!pObj->Init())
		{
			SAFE_DELETE(pObj);
			return nullptr;
		}

		if (pLayer)
		{
			pLayer->AddObject(pObj);
		}

		AddObject(pObj);
		return pObj;
	}
	Object* CreateCloneObject(const string& strPrototypeKey, const string& strTag, class Layer* pLayer);
public:
	Object* FindPrototype(const string& strPrototypeKey);
	void ErasePrototype(const string& strPrototypeKey);
	void EraseAllPrototypes();
	template<typename T>
	T* CreatePrototype(const string& strTag)
	{
		T* pObj = new T;
		pObj->SetTag(strTag);

		if (!pObj->Init())
		{
			SAFE_DELETE(pObj);
			return nullptr;
		}

		pObj->AddRef();
		m_mapProtoType.insert(make_pair(strTag, pObj));

		return pObj;
	}
	Layer* FindLayer(const string& tag);
protected:
	Scene();
	virtual ~Scene() = 0;
	list<Layer*> m_LayerList;
	Layer* CreateLayer(const string& tag, int zOrder = 0);
	static bool LayerSort(const Layer* pL1, const Layer* pL2);
	virtual bool Init();
	virtual void Input(float dt);
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
public:
	static string GetNearObjectName(Scene* pScene, const Pos& worldPos);
	static string ConvertToNameOption(TILE_OPTION eOpt);
protected:
	void LoadStage(const string& objectTag, const string& strlayerTag, FILE* pFile);
	virtual void LoadDefaultStages(const char* fileName);
};

