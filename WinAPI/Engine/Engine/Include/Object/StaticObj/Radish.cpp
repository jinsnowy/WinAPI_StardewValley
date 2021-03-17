#include "Radish.h"

Radish::Radish()
{
}

Radish::~Radish()
{
}

void Radish::TileHit(Collider* pSrc, Collider* pDst, float dt)
{
}

void Radish::Die()
{
    Plant::Die();
}

bool Radish::Init()
{
    LoadGrowTexture(m_plantTag, L"SV/Plant/Radish/");
    SetGrowPeriod(HOURS(10));
    SetGrowTexture(m_plantTag);

    Item* pItem = Item::FindItem("Radish_Fruit");
    SetDropItem(pItem);
    SAFE_RELEASE(pItem);

    return true;
}

void Radish::Input(float dt)
{
    Plant::Input(dt);
}

int Radish::Update(float dt)
{
    Plant::Update(dt);
    return 0;
}

int Radish::LateUpdate(float dt)
{
    Plant::LateUpdate(dt);
    return 0;
}

void Radish::Collision(float dt)
{
    Plant::Collision(dt);
}

void Radish::Draw(HDC hDC, float dt)
{
    Plant::Draw(hDC, dt);
}

void Radish::LateInit()
{
}

void Radish::Save(FILE* pFile)
{
    Plant::Save(pFile);
}

void Radish::Load(FILE* pFile)
{
    Plant::Load(pFile);
}
