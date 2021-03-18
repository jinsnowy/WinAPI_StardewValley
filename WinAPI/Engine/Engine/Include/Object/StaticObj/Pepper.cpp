#include "Pepper.h"

Pepper::Pepper()
{
}

Pepper::~Pepper()
{
}

void Pepper::Die()
{
    Plant::Die();
}

bool Pepper::Init()
{
    LoadGrowTexture(m_plantTag, L"SV/Plant/Pepper/");
    SetGrowPeriod(HOURS(10));
    SetGrowTexture(m_plantTag);
    SetPeriodicPlantHarvestNum(4);
    SetDropItemNum(2);

    Item* pItem = Item::FindItem("Pepper_Fruit");
    SetDropItem(pItem);
    SAFE_RELEASE(pItem);

    return true;
}

void Pepper::Input(float dt)
{
    Plant::Input(dt);
}

int Pepper::Update(float dt)
{
    Plant::Update(dt);
    return 0;
}

int Pepper::LateUpdate(float dt)
{
    Plant::LateUpdate(dt);
    return 0;
}

void Pepper::Collision(float dt)
{
    Plant::Collision(dt);
}

void Pepper::Draw(HDC hDC, float dt)
{
    Plant::Draw(hDC, dt);
}

void Pepper::LateInit()
{
}

void Pepper::Save(FILE* pFile)
{
    Plant::Save(pFile);
}

void Pepper::Load(FILE* pFile)
{
    Plant::Load(pFile);
}
