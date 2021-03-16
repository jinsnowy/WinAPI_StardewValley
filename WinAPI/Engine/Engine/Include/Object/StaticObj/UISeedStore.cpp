#include "UISeedStore.h"
#include "UIPanel.h"
#include "UIButton.h"
#include "UIGameManager.h"
#include "../../Resources/ResourceManager.h"
#include "../../Collider/ColliderRect.h"
#include "../../Collider/CollisionManager.h"
#include "../MoveObj/Player.h"
#include "../Item/Item.h"

UISeedStore::UISeedStore()
{
	Init();
}

UISeedStore::~UISeedStore()
{
	SAFE_RELEASE(m_pPlayer);
	SAFE_RELEASE(m_pExitButton);
	Safe_Release_VecList(m_vecSellingSeeds);
}

bool UISeedStore::Init()
{
	m_vecSellingSeeds = Item::LoadItemFromDirectory(L"SV/Item/Seed/", RGB(255, 255, 255));

	SetTexture("SeedStore", L"SV/Scene/SeedStoreUIMain.bmp");
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

	m_pExitButton = Object::CreateObject<UIButton>("StoreExitButton");
	m_pExitButton->SetTexture("StoreExitButton", L"SV/Scene/ExitButton.bmp");
	m_pExitButton->SetAsTextureSize();
	m_pExitButton->SetPos(GetPos().x + GetSize().x + 50.f, GetPos().y);
	m_pExitButton->SetCallback(this, &UISeedStore::Exit);

	Size tSize = m_pExitButton->GetSize();
	ColliderRect* pRC = static_cast<ColliderRect*>(m_pExitButton->GetCollider("ButtonBody"));
	pRC->SetRect(0, 0, tSize.x, tSize.y);
	SAFE_RELEASE(pRC);
    return true;
}

void UISeedStore::Input(float dt)
{
    UI::Input(dt);
}

int UISeedStore::Update(float dt)
{
    UI::Update(dt);

	m_pExitButton->Update(dt);
	m_pExitButton->LateUpdate(dt);
	COLLISION_MANAGER->AddObject(m_pExitButton);
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
	m_pExitButton->Draw(hdc, dt);

	int st_x = GetPos().x + 35;
	int st_y = GetPos().y + 35;
	int margin = (64 - 54) / 2;
	int num_st_x = GetPos().x + GetSize().x - 200;
	for (int i = m_iCurSel; i < m_iCurSel + 4; ++i)
	{
		m_vecSellingSeeds[i]->DrawImageAtFixedSize(hdc, Pos(st_x + margin, st_y + margin), 50);
		string price = to_string(m_vecSellingSeeds[i]->GetPrice());
		RESOURCE_MANAGER->DrawFontsAtFixedSize(hdc, 
					price, num_st_x, st_y + margin + 5, 30, 40, RIGHT, 4);
		st_y += 95;
	}
}

void UISeedStore::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	if (m_pPlayer)
		m_pPlayer->AddRef();
}

void UISeedStore::Exit(float dt)
{
	UI_MANAGER->SetSeedStore(false);
}
