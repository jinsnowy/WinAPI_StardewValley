#include "UISeedStore.h"
#include "UIPanel.h"
#include "../../Resources/ResourceManager.h"
#include "../../Collider/ColliderRect.h"
#include "../MoveObj/Player.h"
#include "../Item/Item.h"

UISeedStore::UISeedStore()
{
	Init();
}

UISeedStore::~UISeedStore()
{
	SAFE_RELEASE(m_pPlayer);
	Safe_Release_VecList(m_vecSellingSeeds);
}

bool UISeedStore::Init()
{
	m_vecSellingSeeds = Item::LoadItemFromDirectory(L"SV/Item/Seed/", RGB(255, 255, 255));

	SetTexture("SeedStore", L"SV/Scene/SeedStoreUI.bmp");
	SetAsTextureSize();
	SetPos(100.f, 100.f);

	Item::FindItem("CauliflowerSeed")->SetPrice(200);
	Item::FindItem("GarlicSeed")->SetPrice(80);
	Item::FindItem("ParsnipSeed")->SetPrice(40);
	Item::FindItem("PotatoSeed")->SetPrice(100);
	Item::FindItem("RadishSeed")->SetPrice(40);
	Item::FindItem("RhubarbSeed")->SetPrice(60);
	Item::FindItem("TomatoSeed")->SetPrice(60);

	sort(m_vecSellingSeeds.begin(), m_vecSellingSeeds.end(), Item::SortByName);

    return true;
}

void UISeedStore::Input(float dt)
{
    UI::Input(dt);
}

int UISeedStore::Update(float dt)
{
    UI::Update(dt);
    return 0;
}

int UISeedStore::LateUpdate(float dt)
{
    UI::LateUpdate(dt);
    return 0;
}

void UISeedStore::Collision(float dt)
{
    UI::Collision(dt);
}

void UISeedStore::Draw(HDC hdc, float dt)
{
    UI::Draw(hdc, dt);
}

void UISeedStore::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	if (m_pPlayer)
		m_pPlayer->AddRef();
}
