#include "Tree.h"
#include "TreeTrunk.h"
#include "../../Resources/ResourceManager.h"
#include "../../Collider/ColliderRect.h"
#include "../../Scene/GameScene.h"
#include "../../Object/MoveObj/Player.h"
#include "../../Sound/SoundManager.h"
#include "../../Resources/Texture.h"
#include "../../Scene/Scene.h"
#include "../../Effect/ShakeEffect.h"

const wchar_t* const Tree::m_strBaseName[] = { L"Tree1.bmp", L"Tree2.bmp", L"Tree3.bmp" };

Tree::Tree()
{
	m_eObjType = OBJ_TREE;
}

Tree::Tree(const Tree& obj)
	: InteractiveTile(obj)
{

}

Tree::~Tree()
{
}

bool Tree::Init()
{
	wchar_t path[MAX_PATH] = { 0 };
	wcscat_s(path, m_strFolderPath);
	wcscat_s(path, m_strBaseName[0]);

	SetTexture("TreeTexture1", path);
	SetColorKey(255, 255, 255);
	SetAsTextureSize();

	SetPos(0.f, 0.f);
	SetPivot(0.3333f, 1.0f);

	SetHP(500.f);

	Item* pItem = Item::CreateCloneItem<Item>("Wood", L"SV/Item/Outdoor/Wood.bmp");
	SetDropItem(pItem);
	SAFE_RELEASE(pItem);

	Size imgSize = GetImageSize();
	ColliderRect* pRC = AddCollider<ColliderRect>("TreeBody");
	pRC->SetRect(-imgSize.x / 3 + 5, -imgSize.y + TILESIZE, 2 * imgSize.x / 3 - 5, - 5);
	SAFE_RELEASE(pRC);

	ColliderRect* pBlock = AddCollider<ColliderRect>("TileBlock");
	pBlock->SetRect(0, 0.f, imgSize.x / 3, TILESIZE);
	SAFE_RELEASE(pBlock);
	return true;
}

void Tree::ChangeTexture(int id)
{
	assert(id < m_iTypeNum);
	string strTexKey = "TreeTexture" + to_string(id + 1);
	wchar_t path[MAX_PATH] = { 0 };
	wcscat_s(path, m_strFolderPath);
	wcscat_s(path, m_strBaseName[id]);
	SetTexture(strTexKey, path);
	SetColorKey(255, 255, 255);
}

void Tree::ShadeIn(Collider* pSrc, Collider* pDst, float dt)
{
	if (pDst->GetTag() == "PlayerBody")
	{
		ColliderRect* pRC_tree = static_cast<ColliderRect*>(pSrc);
		ColliderRect* pRC_player = static_cast<ColliderRect*>(pDst);
		if (pRC_tree->GetWorldInfo().bottom > pRC_player->GetWorldInfo().bottom)
		{
			EnableTransparentEffect();
		}
	}
}

void Tree::TileHit(Collider* pSrc, Collider* pDst, float dt)
{
	if (pSrc->GetTag() == "TileBlock" && pDst->GetTag() == "AxeTool")
	{
		SetEffect(make_shared<ShakeEffect>(this, TOOLSPEED - 0.1f, 0.01f, 5.f, 0.f));

		GetDamageFromPlayerTool();
		CheckDie();

		SOUND_MANAGER->PlaySound("TreeHit");
	}

}

void Tree::AfterDie()
{
	ItemDrop(10);
	Object* pTrunk = m_pScene->CreateObject<TreeTrunk>("TreeTrunk", m_pLayer);
	pTrunk->SetPos(GetPos());
	pTrunk->LateInit();
	SAFE_RELEASE(pTrunk);
}

void Tree::Input(float dt)
{
	InteractiveTile::Input(dt);
	DisableTransparentEffect();
}

int Tree::Update(float dt)
{
	InteractiveTile::Update(dt);
	return 0;
}

int Tree::LateUpdate(float dt)
{
	InteractiveTile::LateUpdate(dt);
	return 0;
}

void Tree::Collision(float dt)
{
	InteractiveTile::Collision(dt);
}

void Tree::Draw(HDC hDC, float dt)
{
	InteractiveTile::Draw(hDC, dt);
}

Tree* Tree::Clone()
{
	return new Tree(*this);
}

void Tree::LateInit()
{
	Size imgSize = GetImageSize();

	Collider* pRC = GetCollider("TreeBody");
	pRC->AddCollisionFunction(CS_ENTER, this, &Tree::ShadeIn);
	pRC->AddCollisionFunction(CS_STAY, this, &Tree::ShadeIn);
	SAFE_RELEASE(pRC);

	pRC = GetCollider("TileBlock");
	pRC->AddCollisionFunction(CS_ENTER, this, &Tree::TileHit);
	pRC->AddCollisionFunction(CS_STAY, this, &Tree::TileHit);
	SAFE_RELEASE(pRC);
}

void Tree::Save(FILE* pFile)
{
	InteractiveTile::Save(pFile);
}

void Tree::Load(FILE* pFile)
{
	InteractiveTile::Load(pFile);
}
