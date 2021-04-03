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
#include "../Object/StaticObj/Npc.h"
#include "../Object/Item/Item.h"
#include "../Object/Item/Seed.h"
#include "../Object/MoveObj/Monster.h"

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
    Texture* pTex = RESOURCE_MANAGER->LoadTexture("Dirt_Wet", L"SV/TileGround/Outdoor/Dirt_Wet.bmp");
    SAFE_RELEASE(pTex);

    vector<Item*> m_seedItem = Item::LoadItemFromDirectory<Seed>(L"SV/Item/Seed/", RGB(255, 255, 255));
    for (Item* pItem : m_seedItem)
    {
        Seed* pSeed = static_cast<Seed*>(pItem);
        const string& seedName = pSeed->GetTag();
        if (seedName == "Cauliflower_Seed")
        {
            pSeed->SetPrice(200);
            pSeed->SetPlantTag("Cauliflower_Plant");
        }
        else if (seedName == "Garlic_Seed")
        {
            pSeed->SetPrice(60);
            pSeed->SetPlantTag("Garlic_Plant");
        }
        else if (seedName == "Parsnip_Seed")
        {
            pSeed->SetPrice(40);
            pSeed->SetPlantTag("Parsnip_Plant");
        }
        else if (seedName == "Radish_Seed")
        {
            pSeed->SetPrice(40);
            pSeed->SetPlantTag("Radish_Plant");
        }
        else if (seedName == "Pepper_Seed")
        {
            pSeed->SetPrice(80);
            pSeed->SetPlantTag("Pepper_Plant");
        }
        else if (seedName == "BlueBerry_Seed")
        {
            pSeed->SetPrice(100);
            pSeed->SetPlantTag("BlueBerry_Plant");
        }
        else if (seedName == "Tomato_Seed")
        {
            pSeed->SetPrice(80);
            pSeed->SetPlantTag("Tomato_Plant");
        }
    }
    Safe_Release_VecList(m_seedItem);

    Item* pItem = Item::CreateCloneItem<Item>("Fiber", L"SV/Item/Outdoor/Fiber.bmp");
    pItem->SetItemSellPrice(5);
    SAFE_RELEASE(pItem);
    pItem = Item::CreateCloneItem<Item>("Stone", L"SV/Item/Outdoor/Stone.bmp");
    pItem->SetItemSellPrice(5);
    SAFE_RELEASE(pItem);
    pItem = Item::CreateCloneItem<Item>("Wood", L"SV/Item/Outdoor/Wood.bmp");
    pItem->SetItemSellPrice(5);
    SAFE_RELEASE(pItem);

    vector<Item*>m_vecItem = Item::LoadItemFromDirectory<Item>(L"SV/Item/Fruit/", RGB(255, 255, 255));
    for (Item* pFruit : m_vecItem)
    {
        const string& fruitName = pFruit->GetTag();
        if (fruitName == "Cauliflower_Fruit")
        {
            pFruit->SetItemSellPrice(300);
        }
        else if (fruitName == "Garlic_Fruit")
        {
            pFruit->SetItemSellPrice(100);
        }
        else if (fruitName == "Parsnip_Fruit")
        {
            pFruit->SetItemSellPrice(80);
        }
        else if (fruitName == "Radish_Fruit")
        {
            pFruit->SetItemSellPrice(80);
        }
        else if (fruitName == "Pepper_Fruit")
        {
            pFruit->SetItemSellPrice(30);
        }
        else if (fruitName == "BlueBerry_Fruit")
        {
            pFruit->SetItemSellPrice(30);
        }
        else if (fruitName == "Tomato_Fruit")
        {
            pFruit->SetItemSellPrice(40);
        }
    }

    Safe_Release_VecList(m_vecItem);

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

    Npc* pObj = Object::CreateObject<Npc>("NPC");
    RegisterProtoType(PR_OUTDOOR, "NPC_Prototype", pObj);

    Monster* pMonster = Object::CreateObject<Monster>("Monster");
    RegisterProtoType(PR_OUTDOOR, "Monster_Prototype", pMonster);
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

