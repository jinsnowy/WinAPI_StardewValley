#include "Cauliflower.h"

Cauliflower::Cauliflower()
{
}

Cauliflower::~Cauliflower()
{
}

bool Cauliflower::Init()
{
    LoadGrowTexture(m_plantTag, L"SV/Plant/Cauliflower/");
    SetGrowPeriod(HOURS(15));
    SetGrowTexture(m_plantTag);

    Item* pItem = Item::FindItem("Cauliflower_Fruit");
    SetDropItem(pItem);
    SAFE_RELEASE(pItem);

    return true;
}

void Cauliflower::Input(float dt)
{
    Plant::Input(dt);
}

int Cauliflower::Update(float dt)
{
    Plant::Update(dt);
    return 0;
}

int Cauliflower::LateUpdate(float dt)
{
    Plant::LateUpdate(dt);
    return 0;
}

void Cauliflower::Collision(float dt)
{
    Plant::Collision(dt);
}

void Cauliflower::Draw(HDC hDC, float dt)
{
    Plant::Draw(hDC, dt);
}

void Cauliflower::LateInit()
{
}

void Cauliflower::Save(FILE* pFile)
{
    Plant::Save(pFile);
}

void Cauliflower::Load(FILE* pFile)
{
    Plant::Load(pFile);
}
