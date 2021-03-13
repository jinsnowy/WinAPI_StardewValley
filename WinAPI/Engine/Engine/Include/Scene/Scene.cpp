#include "Scene.h"
#include "Layer.h"
#include "../Math.h"
#include "../Object/Object.h"
#include "../Object/StaticObj/Stage.h"
#include "../Scene/SceneManager.h"
#include "../Application/Window.h"
#include "../Core/Camera.h"
#include "../Core/PathManager.h"
Scene::Scene()
{
	Layer* pLayer = CreateLayer("UI", INT_MAX);
	pLayer = CreateLayer("HUD", INT_MAX - 1);
	pLayer = CreateLayer("Static", 6);
	pLayer = CreateLayer("Effect", 3);
	pLayer = CreateLayer("Object", 2);
	pLayer = CreateLayer("Ground", 0);
}

Scene::~Scene()
{
	EraseAllPrototypes();
	Safe_Delete_VecList(m_LayerList);
	Safe_Release_VecList(m_ObjList);
}

Object* Scene::FindObject(const string& tag)
{
	auto iterEnd = m_ObjList.end();
	for (auto it = m_ObjList.begin(); it != iterEnd; ++it)
	{
		if ((*it)->GetTag() == tag)
		{
			return *it;
		}
	}
	return nullptr;
}

void Scene::EraseObject(Object* pObj)
{
	list<Object*>::iterator it;
	list<Object*>::iterator iterEnd = m_ObjList.end();

	const auto ptr = &m_ObjList;
	for (it = m_ObjList.begin(); it != iterEnd; ++it)
	{
		if (*it == pObj)
		{
			SAFE_RELEASE((*it));
			m_ObjList.erase(it);
			return;
		}
	}
}

void Scene::EraseObject(const string& tag)
{
	auto iterEnd = m_ObjList.end();
	for (auto it = m_ObjList.begin(); it != iterEnd; ++it)
	{
		if ((*it)->GetTag() == tag)
		{
			SAFE_RELEASE((*it));
			it = m_ObjList.erase(it);
			return;
		}
	}
}

void Scene::EraseAllObjects()
{
	Safe_Release_VecList(m_ObjList);
}

void Scene::AddObject(Object* pObj, Layer* pLayer)
{
	if (pLayer)
	{
		pLayer->AddObject(pObj);
	}

	pObj->AddRef();
	m_ObjList.push_back(pObj);
}

Object* Scene::CreateCloneObject(const string& strPrototypeKey, const string& strTag, Layer* pLayer)
{
	Object* pObj = FindPrototype(strPrototypeKey);
	if (pObj == nullptr)
		return nullptr;

	pObj = pObj->Clone();
	pObj->SetTag(strTag);

	if (pLayer)
	{
		pLayer->AddObject(pObj);
	}

	AddObject(pObj);
	return pObj;
}

Object* Scene::FindPrototype(const string& strPrototypeKey)
{
	auto found = m_mapProtoType.find(strPrototypeKey);
	if (found != m_mapProtoType.end())
	{
		found->second->AddRef();
		return found->second;
	}
	return nullptr;
}

void Scene::ErasePrototype(const string& strPrototypeKey)
{
	auto found = m_mapProtoType.find(strPrototypeKey);
	if (found != m_mapProtoType.end())
		m_mapProtoType.erase(strPrototypeKey);
	return;
}

void Scene::EraseAllPrototypes()
{
	Safe_Release_Map(m_mapProtoType);
}

Layer* Scene::FindLayer(const string& tag)
{
	for (auto it = m_LayerList.begin(); it != m_LayerList.end(); it++)
	{
		if ((*it)->GetTag() == tag)
			return *it;
	}
	return nullptr;
}

Layer* Scene::CreateLayer(const string& tag, int zOrder)
{
	Layer* pLayer = new Layer;

	pLayer->SetTag(tag);
	pLayer->SetZOrder(zOrder);
	pLayer->SetScene(this);

	m_LayerList.push_back(pLayer);
	
	if (m_LayerList.size() >= 2)
	{
		m_LayerList.sort(LayerSort);
	}
	return pLayer;
}

bool Scene::LayerSort(const Layer* pL1, const Layer* pL2)
{
	return pL1->GetZOrder() < pL2->GetZOrder();
}

bool Scene::Init()
{
	return true;
}

void Scene::Input(float dt)
{
	auto iterEnd = m_LayerList.end();
	for (auto it = m_LayerList.begin(); it != iterEnd;)
	{
		if (!(*it)->GetEnable())
		{
			++it;
			continue;
		}

		(*it)->Input(dt);

		if (!(*it)->GetLife())
		{
			SAFE_DELETE(*it);
			it = m_LayerList.erase(it);
			iterEnd = m_LayerList.end();
		}
		else ++it;
	}
}

void Scene::Update(float dt)
{
	auto iterEnd = m_LayerList.end();
	for (auto it = m_LayerList.begin(); it != iterEnd;)
	{
		if (!(*it)->GetEnable())
		{
			++it;
			continue;
		}

		(*it)->Update(dt);

		if (!(*it)->GetLife())
		{
			SAFE_DELETE(*it);
			it = m_LayerList.erase(it);
			iterEnd = m_LayerList.end();
		}
		else ++it;
	}
}

void Scene::LateUpdate(float dt)
{
	auto iterEnd = m_LayerList.end();
	for (auto it = m_LayerList.begin(); it != iterEnd;)
	{
		if (!(*it)->GetEnable())
		{
			++it;
			continue;
		}

		(*it)->LateUpdate(dt);

		if (!(*it)->GetLife())
		{
			SAFE_DELETE(*it);
			it = m_LayerList.erase(it);
			iterEnd = m_LayerList.end();
		}
		else ++it;
	}
}

void Scene::Collision(float dt)
{
	auto iterEnd = m_LayerList.end();
	for (auto it = m_LayerList.begin(); it != iterEnd;)
	{
		if (!(*it)->GetEnable())
		{
			++it;
			continue;
		}

		(*it)->Collision(dt);

		if (!(*it)->GetLife())
		{
			SAFE_DELETE(*it);
			it = m_LayerList.erase(it);
			iterEnd = m_LayerList.end();
		}
		else ++it;
	}
}

void Scene::Draw(HDC hdc, float dt)
{
	DrawHDCWithColor(hdc, GETRESOLUTION.x, GETRESOLUTION.y, RGB(0, 0, 0));

	auto iterEnd = m_LayerList.end();
	for (auto it = m_LayerList.begin(); it != iterEnd;)
	{
#ifdef _DEBUG
		const string& layerTag = (*it)->GetTag();
#endif

		if (!(*it)->GetEnable())
		{
			++it;
			continue;
		}

		(*it)->Draw(hdc, dt);

		if (!(*it)->GetLife())
		{
			SAFE_DELETE(*it);
			it = m_LayerList.erase(it);
			iterEnd = m_LayerList.end();
		}
		else ++it;
	}
}

string Scene::GetNearObjectName(Scene* pScene, const Pos& worldPos)
{
	float minDist = FLT_MAX;
	Object* pObj = nullptr;

	Layer* pLayer = pScene->FindLayer("Object");
	const auto& objList = pLayer->GetObjList();
	auto iterEnd = objList->end();
	for (auto iter = objList->begin(); iter != iterEnd; ++iter)
	{
		float dist = Math::Distance((*iter)->GetPos(), worldPos);
		if (dist < TILESIZE / 2 && dist < minDist)
		{
			pObj = (*iter);
			minDist = dist;
		}
	}
	if (pObj)
	{
		char buffer[30] = { 0 };
		sprintf_s(buffer, "Pos (%.1f,%.1f)", pObj->GetPos().x, pObj->GetPos().y);
		return pObj->GetTag() + string(buffer);
	}
	return "";

}

string Scene::ConvertToNameOption(TILE_OPTION eOpt)
{
	switch (eOpt)
	{
	case TO_NONE:
		return "NoOption";
	case TO_NOMOVE:
		return "NoMove";
	case TO_CROP_GROUND:
		return "CropGround";
	case TO_BEACON_1:
		return "Beacon1";
	case TO_BEACON_2:
		return "Beacond";
	case TO_BEACON_3:
		return "Beacon3";
	default:
		return "Invalid";
	}
}

void Scene::LoadStage(const string& objectTag, const string& strlayerTag, FILE* pFile)
{
	Layer* pStageLayer = FindLayer(strlayerTag);
	Stage* pStage = CreateObject<Stage>(objectTag, pStageLayer);
	pStage->LoadFromFile(pFile);
	SAFE_RELEASE(pStage);
}

void Scene::LoadDefaultStages(const char* fileName)
{
	FILE* pFile = PATH_MANAGER->FileOpen(fileName, DATA_PATH, "rb");

	LoadStage("GroundStage", "Ground", pFile);
	LoadStage("StaticStage", "Static", pFile);

	Layer* pLayer = FindLayer("Object");
	int objNum, objType;
	fread(&objNum, 4, 1, pFile);
	for (int i = 0; i < objNum; ++i)
	{
		fread(&objType, 4, 1, pFile);
		Object* pObj = Object::CreateObjectByType(static_cast<OBJ_TYPE>(objType));
		if (pObj)
		{
			pObj->Load(pFile);
			AddObject(pObj, pLayer);
			SAFE_RELEASE(pObj);
		}
	}

	if (pFile)
	{
		fclose(pFile);
	}
}
