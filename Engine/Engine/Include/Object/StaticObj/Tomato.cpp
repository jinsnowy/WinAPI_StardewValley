#include "Tomato.h"

Tomato::Tomato()
{
}

Tomato::~Tomato()
{
}

bool Tomato::Init()
{
    LoadGrowTexture(m_plantTag, L"SV/Plant/Tomato/");
    SetGrowPeriod(HOURS(10));
    SetGrowTexture(m_plantTag);
    SetPeriodicPlantHarvestNum(5);

    Item* pItem = Item::FindItem("Tomato_Fruit");
    SetDropItem(pItem);
    SAFE_RELEASE(pItem);

    return true;
}

void Tomato::Input(float dt)
{
    Plant::Input(dt);
}

int Tomato::Update(float dt)
{
    Plant::Update(dt);
    return 0;
}

int Tomato::LateUpdate(float dt)
{
    Plant::LateUpdate(dt);
    return 0;
}

void Tomato::Collision(float dt)
{
    Plant::Collision(dt);
}

void Tomato::Draw(HDC hDC, float dt)
{
    Plant::Draw(hDC, dt);
}

void Tomato::LateInit()
{
}

void Tomato::Save(FILE* pFile)
{
    Plant::Save(pFile);
}

void Tomato::Load(FILE* pFile)
{
    Plant::Load(pFile);
}
