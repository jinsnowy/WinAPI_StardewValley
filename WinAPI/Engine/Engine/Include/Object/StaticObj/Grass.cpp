#include "Grass.h"
#include "../../Resources/ResourceManager.h"
#include "../../Collider/ColliderRect.h"
#include "../../Sound/SoundManager.h"
#include "../../Resources/Texture.h"
const wchar_t* const Grass::m_strBaseName[] = { L"Grass1.bmp", L"Grass2.bmp", L"Grass3.bmp" };

Grass::Grass()
{
	m_eObjType = OBJ_GRASS;
}

Grass::Grass(const Grass& obj)
	: InteractiveTile(obj)
{

}

Grass::~Grass()
{
}

bool Grass::Init()
{
	wchar_t path[MAX_PATH] = { 0 };
	wcscat_s(path, m_strFolderPath);
	wcscat_s(path, m_strBaseName[0]);

	SetTexture("GrassTexture1", path);
	SetColorKey(255, 255, 255);
	SetAsTextureSize();

	SetPos(0.f, 0.f);
	SetPivot(0.f, 0.f);

	SetHP(50.f);

	ColliderRect* pBlock = AddCollider<ColliderRect>("TileIdleBlock");
	pBlock->SetRect(5.f, 5.f, TILESIZE - 5.f, TILESIZE - 5.f);
	SAFE_RELEASE(pBlock);
	return true;
}

void Grass::ChangeTexture(int id)
{
	assert(id < m_iTypeNum);
	string strTexKey = "GrassTexture" + to_string(id + 1);
	wchar_t path[MAX_PATH] = { 0 };
	wcscat_s(path, m_strFolderPath);
	wcscat_s(path, m_strBaseName[id]);
	SetTexture(strTexKey, path);
	SetColorKey(255, 255, 255);
}


void Grass::TileHit(Collider* pSrc, Collider* pDst, float dt)
{
	if (pDst->GetTag() == "SwingTool")
	{
		Die();
	}
}

void Grass::Die()
{
	Ref::Die();

	SOUND_MANAGER->PlaySound("GrassOver");
}

void Grass::Input(float dt)
{
	InteractiveTile::Input(dt);
	DisableTransparentEffect();
}

int Grass::Update(float dt)
{
	InteractiveTile::Update(dt);
	return 0;
}

int Grass::LateUpdate(float dt)
{
	InteractiveTile::LateUpdate(dt);
	return 0;
}

void Grass::Collision(float dt)
{
	InteractiveTile::Collision(dt);
}

void Grass::Draw(HDC hDC, float dt)
{
	InteractiveTile::Draw(hDC, dt);
}

Grass* Grass::Clone()
{
	return new Grass(*this);
}

void Grass::Save(FILE* pFile)
{
	InteractiveTile::Save(pFile);
}

void Grass::Load(FILE* pFile)
{
	InteractiveTile::Load(pFile);

	Size imgSize = GetImageSize();

	Collider* pRC = static_cast<ColliderRect*>(GetCollider("TileIdleBlock"));
	pRC->AddCollisionFunction(CS_ENTER, this, &Grass::TileHit);
	pRC->AddCollisionFunction(CS_STAY, this, &Grass::TileHit);
	SAFE_RELEASE(pRC);
}
