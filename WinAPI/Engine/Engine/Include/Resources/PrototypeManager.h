#pragma once
#include "../framework.h"

class Item;
class Object;
class Layer;
class PrototypeManager
{
	friend class Object;
	friend class UITileSelect;
	DECLARE_SINGLE(PrototypeManager);
private:
	static unordered_map<string, Object*> m_mapObjPrototype[PR_END];

	unordered_map<string, Object*>* GetPrototypes(PR_TYPE eType) const { return &m_mapObjPrototype[eType]; }
public:
	template<typename T>
	static T* LoadObject()
	{
		T* pObj = new T;
		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return nullptr;
		}
		return pObj;
	}
	template<typename T>
	static void RegisterProtoType(PR_TYPE eType, const string& strPrototypeKey)
	{
		if (FindPrototype(eType, strPrototypeKey))
			throw EXCEPT(L"[PrototyeManager] Prototype string already exists");
		T* pObj = LoadObject<T>();
		if (pObj)
		{
			RegisterProtoType(eType, strPrototypeKey, pObj);
		}
	}
	static void RegisterProtoType(PR_TYPE eType, const string& strPrototypeKey, Object* pObj);
	static Object* FindPrototype(const string& strPrototypeKey);
	static Object* FindPrototype(PR_TYPE eType, const string& strPrototypeKey);
	static Object* CreateCloneObject(const string& strPrototypeKey, const string& strTag, class Scene* pScene = nullptr, class Layer* pLayer = nullptr);

	bool Init();
};

