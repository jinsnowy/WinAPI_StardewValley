#include "Parsnip.h"

Parsnip::Parsnip()
{
}

Parsnip::~Parsnip()
{
}

void Parsnip::TileHit(Collider* pSrc, Collider* pDst, float dt)
{
}

void Parsnip::Die()
{
    Plant::Die();
}

bool Parsnip::Init()
{
    LoadGrowTexture(m_plantTag, L"SV/Plant/Parsnip/");
    SetGrowPeriod(HOURS(10));

    SetGrowTexture(m_plantTag);
 
    Item* pItem  = Item::FindItem("Parsnip_Fruit");
    SetDropItem(pItem);
    SAFE_RELEASE(pItem);

    return true;
}

void Parsnip::Input(float dt)
{
    Plant::Input(dt);
}

int Parsnip::Update(float dt)
{
    Plant::Update(dt);
    return 0;
}

int Parsnip::LateUpdate(float dt)
{
    Plant::LateUpdate(dt);
    return 0;
}

void Parsnip::Collision(float dt)
{
    Plant::Collision(dt);
}

void Parsnip::Draw(HDC hDC, float dt)
{
    Plant::Draw(hDC, dt);
}

void Parsnip::LateInit()
{
}

void Parsnip::Save(FILE* pFile)
{
    Plant::Save(pFile);
}

void Parsnip::Load(FILE* pFile)
{
    Plant::Load(pFile);
}
