#include "TreeTrunk.h"
#include "../../Resources/ResourceManager.h"
#include "../../Collider/ColliderRect.h"
#include "../../Sound/SoundManager.h"
#include "../../Resources/Texture.h"
#include "../../Scene/GameScene.h"
#include "../../Object/MoveObj/Player.h"
const wchar_t* const TreeTrunk::m_strBaseName[] = { L"TreeTrunk1.bmp"};

TreeTrunk::TreeTrunk()
{
	m_eObjType = OBJ_TREETRUNK;
}

TreeTrunk::TreeTrunk(const TreeTrunk& obj)
	: InteractiveTile(obj)
{

}

TreeTrunk::~TreeTrunk()
{
}

bool TreeTrunk::Init()
{
	wchar_t path[MAX_PATH] = { 0 };
	wcscat_s(path, m_strFolderPath);
	wcscat_s(path, m_strBaseName[0]);

	SetTexture("TreeTrunkTexture1", path);
	SetColorKey(255, 255, 255);
	SetAsTextureSize();

	SetPos(0.f, 0.f);
	SetPivot(0.f, 0.5f);

	SetHP(300.f);

	Item* pItem = Item::LoadItem("Wood", L"SV/Item/Outdoor/Wood.bmp");
	SetDropItem(pItem);
	SAFE_RELEASE(pItem);

	AdvertiseFrom(CO_OBJECT);
	ListenTo(CO_PLAYER);

	ColliderRect* pBlock = AddCollider<ColliderRect>("TileBlock");
	pBlock->SetRect(5.f, 5.f, TILESIZE - 5.f, TILESIZE - 5.f);
	SAFE_RELEASE(pBlock);

	return true;
}


void TreeTrunk::TileHit(Collider* pSrc, Collider* pDst, float dt)
{
	if (pSrc->GetTag() == "TileBlock" && pDst->GetTag() == "AxeTool")
	{
		float power = static_cast<GameScene*>(m_pScene)->AccessPlayer()->GetToolPower();
		GetDamage(power);
		SOUND_MANAGER->PlaySound("TreeHit");
		if (IsDie())
		{
			Die();
		}
	}
}

void TreeTrunk::Die()
{
	Ref::Die();

	ItemDrop(3);
}

void TreeTrunk::Input(float dt)
{
	InteractiveTile::Input(dt);
}

int TreeTrunk::Update(float dt)
{
	InteractiveTile::Update(dt);
	return 0;
}

int TreeTrunk::LateUpdate(float dt)
{
	InteractiveTile::LateUpdate(dt);
	return 0;
}

void TreeTrunk::Collision(float dt)
{
	InteractiveTile::Collision(dt);
}

void TreeTrunk::Draw(HDC hDC, float dt)
{
	InteractiveTile::Draw(hDC, dt);
}

TreeTrunk* TreeTrunk::Clone()
{
	return new TreeTrunk(*this);
}

void TreeTrunk::LateInit()
{
	Size imgSize = GetImageSize();
	Collider* pRC = static_cast<ColliderRect*>(GetCollider("TileBlock"));
	pRC->AddCollisionFunction(CS_ENTER, this, &TreeTrunk::TileHit);
	pRC->AddCollisionFunction(CS_STAY, this, &TreeTrunk::TileHit);
	SAFE_RELEASE(pRC);
}

void TreeTrunk::Save(FILE* pFile)
{
	InteractiveTile::Save(pFile);
}

void TreeTrunk::Load(FILE* pFile)
{
	InteractiveTile::Load(pFile);
}
