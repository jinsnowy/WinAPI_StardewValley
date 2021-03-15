#include "UIGameManager.h"
#include "UIPanel.h"
#include "../../Application/Window.h"
#include "../../Core/Input.h"
#include "../../Resources/Texture.h"
#include "../../Collider/ColliderRect.h"
#include "../Object.h"
#include "../MoveObj/Player.h"
#include "../Item/Item.h"


DEFINITION_SINGLE(UIGameManager);

bool UIGameManager::Init()
{
	return true;
}

void UIGameManager::Input(float dt)
{
}

int UIGameManager::Update(float dt)
{
	if (m_bTickStart)
	{
		m_clockPanel->Update(dt);
	}
	m_itemPanel->Update(dt);
    return 0;
}

int UIGameManager::LateUpdate(float dt)
{
	m_clockPanel->LateUpdate(dt);
	m_itemPanel->LateUpdate(dt);
    return 0;
}

void UIGameManager::Collision(float dt)
{
	m_clockPanel->Collision(dt);
	m_itemPanel->Collision(dt);
}

void UIGameManager::Draw(HDC hdc, float dt)
{
	m_clockPanel->Draw(hdc, dt);
	if (m_bFastItemListSelect)
	{
		m_itemPanel->Draw(hdc, dt);
	}
}

void UIGameManager::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	if (m_pPlayer)
		m_pPlayer->AddRef();

	m_clockPanel->SetPlayer(pPlayer);
	m_itemPanel->SetPlayer(pPlayer);
	m_storePanel->SetPlayer(pPlayer);
}


UIGameManager::UIGameManager()
{

}

UIGameManager::~UIGameManager()
{
	SAFE_RELEASE(m_pPlayer);
}