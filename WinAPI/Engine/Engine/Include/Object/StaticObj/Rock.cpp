#include "Rock.h"
#include "../../Resources/ResourceManager.h"
#include "../../Collider/ColliderRect.h"
#include "../../Sound/SoundManager.h"
#include "../../Resources/Texture.h"
const wchar_t* const Rock::m_strBaseName[] = { L"Rock1.bmp", L"Rock2.bmp", L"Rock3.bmp" };

Rock::Rock()
{
	m_eObjType = OBJ_ROCK;
}

Rock::Rock(const Rock& obj)
	: InteractiveTile(obj)
{

}

Rock::~Rock()
{
}

bool Rock::Init()
{
	wchar_t path[MAX_PATH] = { 0 };
	wcscat_s(path, m_strFolderPath);
	wcscat_s(path, m_strBaseName[0]);

	SetTexture("RockTexture1", path);
	SetColorKey(255, 255, 255);
	SetAsTextureSize();

	SetPos(0.f, 0.f);
	SetPivot(0.f, 0.f);

	SetHP(50.f);

	ColliderRect* pBlock = AddCollider<ColliderRect>("TileBlock");
	pBlock->SetRect(5.f, 5.f, TILESIZE-5.f, TILESIZE-5.f);
	SAFE_RELEASE(pBlock);
	return true;
}

void Rock::ChangeTexture(int id)
{
	assert(id < m_iTypeNum);
	string strTexKey = "RockTexture" + to_string(id + 1);
	wchar_t path[MAX_PATH] = { 0 };
	wcscat_s(path, m_strFolderPath);
	wcscat_s(path, m_strBaseName[id]);
	SetTexture(strTexKey, path);
	SetColorKey(255, 255, 255);
}


void Rock::TileHit(Collider* pSrc, Collider* pDst, float dt)
{
	if (pSrc->GetTag() == "TileBlock" && pDst->GetTag() == "PickTool")
	{
		Die();
	}
}

void Rock::Die()
{
	Ref::Die();

	SOUND_MANAGER->PlaySound("RockOver");
}

void Rock::Input(float dt)
{
	InteractiveTile::Input(dt);
	DisableTransparentEffect();
}

int Rock::Update(float dt)
{
	InteractiveTile::Update(dt);
	return 0;
}

int Rock::LateUpdate(float dt)
{
	InteractiveTile::LateUpdate(dt);
	return 0;
}

void Rock::Collision(float dt)
{
	InteractiveTile::Collision(dt);
}

void Rock::Draw(HDC hDC, float dt)
{
	InteractiveTile::Draw(hDC, dt);
}

Rock* Rock::Clone()
{
	return new Rock(*this);
}

void Rock::Save(FILE* pFile)
{
	InteractiveTile::Save(pFile);
}

void Rock::Load(FILE* pFile)
{
	InteractiveTile::Load(pFile);

	Size imgSize = GetImageSize();

	Collider* pRC = static_cast<ColliderRect*>(GetCollider("TileBlock"));
	pRC->AddCollisionFunction(CS_ENTER, this, &Rock::TileHit);
	pRC->AddCollisionFunction(CS_STAY, this, &Rock::TileHit);
	SAFE_RELEASE(pRC);
}
