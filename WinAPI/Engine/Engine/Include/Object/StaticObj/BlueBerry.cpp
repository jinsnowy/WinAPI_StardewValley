#include "BlueBerry.h"

BlueBerry::BlueBerry()
{
}

BlueBerry::~BlueBerry()
{
}

void BlueBerry::Die()
{
    Plant::Die();
}

bool BlueBerry::Init()
{
    LoadGrowTexture(m_plantTag, L"SV/Plant/BlueBerry/");
    SetGrowPeriod(HOURS(10));
    SetGrowTexture(m_plantTag);
    SetPeriodicPlantHarvestNum(5);
    SetDropItemNum(4);

    Item* pItem = Item::FindItem("BlueBerry_Fruit");
    SetDropItem(pItem);
    SAFE_RELEASE(pItem);

    return true;
}

void BlueBerry::Input(float dt)
{
    Plant::Input(dt);
}

int BlueBerry::Update(float dt)
{
    Plant::Update(dt);
    return 0;
}

int BlueBerry::LateUpdate(float dt)
{
    Plant::LateUpdate(dt);
    return 0;
}

void BlueBerry::Collision(float dt)
{
    Plant::Collision(dt);
}

void BlueBerry::Draw(HDC hDC, float dt)
{
    Plant::Draw(hDC, dt);
}

void BlueBerry::LateInit()
{
}

void BlueBerry::Save(FILE* pFile)
{
    Plant::Save(pFile);
}

void BlueBerry::Load(FILE* pFile)
{
    Plant::Load(pFile);
}
