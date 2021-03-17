#include "Garlic.h"

Garlic::Garlic()
{
}

Garlic::~Garlic()
{
}

void Garlic::TileHit(Collider* pSrc, Collider* pDst, float dt)
{
}

void Garlic::Die()
{
    Plant::Die();
}

bool Garlic::Init()
{
    LoadGrowTexture(m_plantTag, L"SV/Plant/Garlic/");
    SetGrowPeriod(HOURS(10));
    SetGrowTexture(m_plantTag);

    Item* pItem = Item::FindItem("Garlic_Fruit");
    SetDropItem(pItem);
    SAFE_RELEASE(pItem);

    return true;
}

void Garlic::Input(float dt)
{
    Plant::Input(dt);
}

int Garlic::Update(float dt)
{
    Plant::Update(dt);
    return 0;
}

int Garlic::LateUpdate(float dt)
{
    Plant::LateUpdate(dt);
    return 0;
}

void Garlic::Collision(float dt)
{
    Plant::Collision(dt);
}

void Garlic::Draw(HDC hDC, float dt)
{
    Plant::Draw(hDC, dt);
}

void Garlic::LateInit()
{
}

void Garlic::Save(FILE* pFile)
{
    Plant::Save(pFile);
}

void Garlic::Load(FILE* pFile)
{
    Plant::Load(pFile);
}
