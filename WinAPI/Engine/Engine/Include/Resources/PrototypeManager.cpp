#include "PrototypeManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "../Object/Object.h"
#include "../Scene/Scene.h"
#include "../Scene/Layer.h"
#include "../Core/PathManager.h"
#include "../Object/StaticObj/Tree.h"
#include "../Object/StaticObj/Rock.h"
#include "../Object/StaticObj/Grass.h"
#include "../Object/StaticObj/TreeTrunk.h"

DEFINITION_SINGLE(PrototypeManager);

unordered_map<string, Object*> PrototypeManager::m_mapObjPrototype[PR_END];


PrototypeManager::PrototypeManager()
{
}

PrototypeManager::~PrototypeManager()
{
    for (int i = 0; i < PR_END; ++i)
    {
        Safe_Release_Map(m_mapObjPrototype[static_cast<PR_TYPE>(i)]);
    }

}

bool PrototypeManager::Init()
{
    Tree* pTree = Object::CreateObject<Tree>("Tree");
    RegisterProtoType(PR_OUTDOOR, "Tree1_Prototype", pTree);
    pTree = pTree->Clone();
    pTree->ChangeTexture(1);
    RegisterProtoType(PR_OUTDOOR, "Tree2_Prototype", pTree);
    pTree = pTree->Clone();
    pTree->ChangeTexture(2);
    RegisterProtoType(PR_OUTDOOR, "Tree3_Prototype", pTree);

    Rock* pRock = Object::CreateObject<Rock>("Rock");
    RegisterProtoType(PR_OUTDOOR, "Rock1_Prototype", pRock);
    pRock = pRock->Clone();
    pRock->ChangeTexture(1);
    RegisterProtoType(PR_OUTDOOR, "Rock2_Prototype", pRock);
    pRock = pRock->Clone();
    pRock->ChangeTexture(2);
    RegisterProtoType(PR_OUTDOOR, "Rock3_Prototype", pRock);

    Grass* pGrass = Object::CreateObject<Grass>("Grass");
    RegisterProtoType(PR_OUTDOOR, "Grass1_Prototype", pGrass);
    pGrass = pGrass->Clone();
    pGrass->ChangeTexture(1);
    RegisterProtoType(PR_OUTDOOR, "Grass2_Prototype", pGrass);
    pGrass = pGrass->Clone();
    pGrass->ChangeTexture(2);
    RegisterProtoType(PR_OUTDOOR, "Grass3_Prototype", pGrass);

    TreeTrunk* pTrunk = Object::CreateObject<TreeTrunk>("TreeTrunk");
    RegisterProtoType(PR_OUTDOOR, "TreeTrunk1_Prototype", pTrunk);

	return true;
}

void PrototypeManager::RegisterProtoType(PR_TYPE eType, const string& strPrototypeKey, Object* pObj)
{
    if (FindPrototype(eType, strPrototypeKey))
        throw EXCEPT(L"[PrototyeManager] Prototype string already exists");

    assert(pObj != nullptr);

    m_mapObjPrototype[eType].insert(make_pair(strPrototypeKey, pObj));
}

Object* PrototypeManager::FindPrototype(const string& strPrototypeKey)
{
    Object* pObj;
    for (int i = 0; i < PR_END; ++i)
    {
        pObj = FindPrototype(static_cast<PR_TYPE>(i), strPrototypeKey);
        if (pObj) return pObj;
    }
    return nullptr;
}

Object* PrototypeManager::FindPrototype(PR_TYPE eType, const string& strPrototypeKeytrTag)
{
	auto found = m_mapObjPrototype[eType].find(strPrototypeKeytrTag);
	if (found == m_mapObjPrototype[eType].end())
	{
		return nullptr;
	}
	return found->second;
}

Object* PrototypeManager::CreateCloneObject(const string& strPrototypeKey, const string& strTag, Scene* pScene, Layer* pLayer)
{
    assert(strPrototypeKey.ends_with("_Prototype"));
	Object* pProto = FindPrototype(strPrototypeKey);

	if (!pProto)
		return nullptr;

	// 복제한 인스턴스
	Object* pObj = pProto->Clone();
	pObj->SetTag(strTag);

    if (pScene)
    {
        pScene->AddObject(pObj);
    }

	if (pLayer)
	{
        pLayer->AddObject(pObj);
	}

	return pObj;
}

