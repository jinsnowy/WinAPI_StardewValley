#include "InHouseScene.h"
#include "../Object/MoveObj/Player.h"
#include "../Sound/SoundManager.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"
#include "SceneManager.h"
#include "../Object/StaticObj/UIPanel.h"
#include "../Collider/ColliderRect.h"
#include "../Collider/CollisionManager.h"
#include "../Application/Window.h"
#include "../Core/Input.h"

InHouseScene::InHouseScene()
{
}

InHouseScene::~InHouseScene()
{
	SAFE_RELEASE(m_pSleepPanel);
}

void InHouseScene::SetUpCamera()
{
}

bool InHouseScene::Init()
{
	if (!GameScene::Init())
	{
		return false;
	}

	GameScene::SetUpScene(pGameDataFileName);

	SOUND_MANAGER->PlaySound("LongBGM");

	m_pSleepPanel = Object::CreateObject<UIPanel>("SleepUIPanel");
	m_pSleepPanel->SetTexture("SleepUIPanel", L"SV/Scene/UI/SleepBanner.bmp");
	m_pSleepPanel->SetAsTextureSize();

	Size tSize = m_pSleepPanel->GetSize();
	m_pSleepPanel->SetPos((GETRESOLUTION.x - tSize.x) / 2.f, GETRESOLUTION.y - tSize.y - 100.f);
	Pos tOffset = Pos(20.f, 106.f);

	ColliderRect* pRC = m_pSleepPanel->AddCollider<ColliderRect>("SleepYes");
	pRC->SetUICollider(true);
	pRC->SetRect(tOffset.x, tOffset.y, tOffset.x + 957.f, tOffset.y + 75.f);
	pRC->AddCollisionFunction(CS_ENTER, this, &InHouseScene::SleepCallBack);
	pRC->AddCollisionFunction(CS_STAY, this, &InHouseScene::SleepCallBack);
	SAFE_RELEASE(pRC);

	pRC = m_pSleepPanel->AddCollider<ColliderRect>("SleepNo");
	pRC->SetUICollider(true);
	pRC->SetRect(tOffset.x, tOffset.y + 75.f, tOffset.x + 957.f, tOffset.y + 150.f);
	pRC->AddCollisionFunction(CS_ENTER, this, &InHouseScene::CancelCallBack);
	pRC->AddCollisionFunction(CS_STAY, this, &InHouseScene::CancelCallBack);
	SAFE_RELEASE(pRC);
	m_pSleepPanel->SetEnable(false);
	FindLayer("UI")->AddObject(m_pSleepPanel);

	return true;
}

void InHouseScene::Input(float dt)
{
	GameScene::Input(dt);
}

void InHouseScene::Update(float dt)
{
	GameScene::Update(dt);
}

void InHouseScene::LateUpdate(float dt)
{
	const Pos& curPos = AccessPlayer()->GetCenterPos();
	TILE_OPTION option = GetTileOption(curPos);
	m_bPlayerOut = true;
	switch (option)
	{
	case TO_BEACON_1:
	{
		SceneState state = {};
		state.nextDir = DOWN;
		state.nextBeacon = BC_ONE;
		state.nextScene = SC_FARM;
		SCENE_MANAGER->SignalizeSceneChange(state);
		return;
	}break;
	}
	if (m_tSleepTrigger.IsCollideRect(AccessPlayer()->GetBodyRect()))
	{
		m_bPlayerOut = false;
		if (!m_pSleepPanel->GetEnable() && m_bPlayerOnceOut)
		{
			m_bPlayerOnceOut = false;
			m_pSleepPanel->SetEnable(true);
			m_pPlayer->DisableMovement();
		}
	}
	if (m_bPlayerOut && !m_bPlayerOnceOut)
	{
		m_bPlayerOnceOut = true;
	}
	GameScene::LateUpdate(dt);
}

void InHouseScene::Collision(float dt)
{
	GameScene::Collision(dt);
}

void InHouseScene::Draw(HDC hdc, float dt)
{
	GameScene::Draw(hdc, dt);
	if (m_pSleepPanel->GetEnable())
	{
		DrawRedRect(hdc, (RECT)m_tSelect);
	}
}

void InHouseScene::SleepCallBack(Collider* pSrc, Collider* pDst, float dt)
{
	if (pDst->GetTag() == "Mouse")
	{
		m_tSelect = static_cast<ColliderRect*>(pSrc)->GetWorldInfo();
		if (KEYPRESS("MouseLButton"))
		{
			m_bClick = true;
		}
		if (KEYDOWN("MouseLButton"))
		{
			if (m_bClick)
			{
				m_bClick = false;
				m_pPlayer->EnableMovement();
				m_pSleepPanel->SetEnable(false);
				SCENE_MANAGER->PlayerSleep();
			}
		}
	}
}

void InHouseScene::CancelCallBack(Collider* pSrc, Collider* pDst, float dt)
{
	if (pDst->GetTag() == "Mouse")
	{
		m_tSelect = static_cast<ColliderRect*>(pSrc)->GetWorldInfo();
		if (KEYDOWN("MouseLButton"))
		{
			m_pPlayer->EnableMovement();
			m_pSleepPanel->SetEnable(false);
		}
	}
}