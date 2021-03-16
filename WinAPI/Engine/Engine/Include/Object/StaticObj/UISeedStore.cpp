#include "UISeedStore.h"
#include "UIPanel.h"
#include "UIButton.h"
#include "UIScrollBar.h"
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
	SAFE_RELEASE(m_pScrollBar);
	Safe_Release_VecList(m_vecSellingSeeds);
	Safe_Release_VecList(m_vecSellingPanelBodies);
}

bool UISeedStore::Init()
{
	m_vecSellingSeeds = Item::LoadItemFromDirectory(L"SV/Item/Seed/", RGB(255, 255, 255));

	SetTexture("SeedStore", L"SV/Scene/SeedStoreUIMain.bmp");
	SetAsTextureSize();
	SetPos(100.f, 100.f);

	Item::FindItem("Cauliflower_Seed")->SetPrice(200);
	Item::FindItem("Garlic_Seed")->SetPrice(80);
	Item::FindItem("Parsnip_Seed")->SetPrice(40);
	Item::FindItem("Potato_Seed")->SetPrice(100);
	Item::FindItem("Pepper_Seed")->SetPrice(40);
	Item::FindItem("Radish_Seed")->SetPrice(40);
	Item::FindItem("Rhubarb_Seed")->SetPrice(60);
	Item::FindItem("Tomato_Seed")->SetPrice(60);

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

	m_pScrollBar = Object::CreateObject<UIScrollBar>("StoreScrollBar");
	m_pScrollBar->SetTexture("ScrollBar", L"SV/Scene/ScrollBar.bmp");
	m_pScrollBar->SetAsTextureSize();
	m_pScrollBar->SetPos(GetPos().x + GetSize().x + 60.f, GetPos().y + m_pExitButton->GetSize().y + 25.f);
	int itemNum = (int) m_vecSellingSeeds.size();
	m_pScrollBar->SetNumBlock(itemNum - 3);

	UIButton* pObj = m_pScrollBar->GetScroller();
	pObj->SetTexture("Scroller", L"SV/Scene/Scroller.bmp");
	pObj->SetAsTextureSize();
	pObj->SetPos(m_pScrollBar->GetPos());

	tSize = pObj->GetSize();
	//pRC = static_cast<ColliderRect*>(pObj->GetCollider("ButtonBody"));
	//pRC->SetRect(0, 0, tSize.x, tSize.y);
	//SAFE_RELEASE(pRC);
	SAFE_RELEASE(pObj);

	SetUpItemBuyColliders();
    return true;
}

void UISeedStore::Input(float dt)
{
    UI::Input(dt);
	m_pScrollBar->Input(dt);
}

int UISeedStore::Update(float dt)
{
    UI::Update(dt);

	m_pExitButton->Update(dt);
	m_pExitButton->LateUpdate(dt);
	COLLISION_MANAGER->AddObject(m_pExitButton);

	for (UIButton* pSellingPanel : m_vecSellingPanelBodies)
	{
		pSellingPanel->Update(dt);
		pSellingPanel->LateUpdate(dt);
		COLLISION_MANAGER->AddObject(pSellingPanel);
	}

	m_pScrollBar->Update(dt);
	m_pScrollBar->LateUpdate(dt);
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
	m_pScrollBar->Draw(hdc, dt);

	// 판매 아이템 리스트
	int curBlock = m_pScrollBar->GetCurBlock();
	float st_x = GetPos().x + 35.f;
	float st_y = GetPos().y + 35.f;
	float num_st_x = GetPos().x + GetSize().x - 200.f;
	for (int i = curBlock; i < curBlock + 4; ++i)
	{
		m_vecSellingSeeds[i]->DrawImageAtFixedSize(hdc, Pos(st_x + m_iMargin, st_y + m_iMargin), 50);
		string price = to_string(m_vecSellingSeeds[i]->GetPrice());
		RESOURCE_MANAGER->DrawFontsAtFixedSize(hdc, 
					price, Pos(num_st_x, st_y + m_iMargin + 5.f), 30, 40, RIGHT, 4);

		string itemName = m_vecSellingSeeds[i]->GetTag();
		RESOURCE_MANAGER->DrawFontsAt(hdc,
			itemName, Pos(st_x + m_iMargin + 100, st_y + m_iMargin + 5.f), LEFT);
		st_y += 95;
	}
}

void UISeedStore::BuyingCallback(float dt, int id)
{
	int curBlock = m_pScrollBar->GetCurBlock() + id;
	Item* curItem = m_vecSellingSeeds[curBlock];
	m_pPlayer->BuyItem(curItem);
}

void UISeedStore::SetUpItemBuyColliders()
{
	m_vecSellingPanelBodies.resize(4, nullptr);
	// 판매 아이템 리스트
	float st_x = GetPos().x + 20.f;
	float st_y = GetPos().y + 20.f;
	for (int i = 0; i <  4; ++i)
	{
		m_vecSellingPanelBodies[i] = Object::CreateObject<UIButton>("SellingPanel" + to_string(i + 1));
		m_vecSellingPanelBodies[i]->SetPos(st_x, st_y);
		m_vecSellingPanelBodies[i]->SetSize(m_fPanelWidth, m_fPanelHeight);
		function<void(float)> clickCallBack =
			bind(&UISeedStore::BuyingCallback, this, std::placeholders::_1, i);
		m_vecSellingPanelBodies[i]->SetCallback(clickCallBack);

		ColliderRect* pRC = static_cast<ColliderRect*>(m_vecSellingPanelBodies[i]->GetCollider("ButtonBody"));
		pRC->SetRect(0.f, 0.f, m_fPanelWidth, m_fPanelHeight);
		SAFE_RELEASE(pRC);
		st_y += 95.f;
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
