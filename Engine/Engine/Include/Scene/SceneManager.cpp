#include "SceneManager.h"
#include "StartScene.h"
#include "MapEditScene.h"
#include "MiddlewayScene.h"
#include "InHouseScene.h"
#include "FarmScene.h"
#include "PelicantownScene.h"
#include "TownStoreScene.h"
#include "../Application/Window.h"
#include "../Core/FrameTimer.h"
#include "../Core/Camera.h"
#include "../Core/Input.h"
#include "../Collider/CollisionManager.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"
#include "../Sound/SoundManager.h"
#include "../Object/Object.h"
#include "../Object/MoveObj/Player.h"
#include "../Object/StaticObj/GameManager.h"

DEFINITION_SINGLE(SceneManager)

SceneManager::SceneManager()
	:
	m_pScene(nullptr)
{
	m_vecScene.resize(SC_END, nullptr);
}

SceneManager::~SceneManager()
{
	for (Scene* sc : m_vecScene)
	{
		SAFE_DELETE(sc);
	}
	m_vecScene.clear();
	SAFE_RELEASE(m_pPlayer);

	GAME_MANAGER->Release();
}

bool SceneManager::Init()
{
	INPUT->AddKey("ShowOption", VK_F1);
	INPUT->AddKey("ShowGrid", VK_F2);
	INPUT->AddKey("ShowColl", VK_F3);
	INPUT->AddKey("TimeElapse", VK_CONTROL, 'T');
	// INPUT->AddKey("TimeReset", VK_CONTROL, 'R');

	SOUND_MANAGER->LoadSound("LongBGM", false, SD_BACKGROUND, "LongBGM.mp3");
	SOUND_MANAGER->LoadSound("BGM", true, SD_BACKGROUND, "BGM.mp3");

	// 모든 텍스쳐 초기화
	CreateScene<MapEditScene>(SC_MAPEDIT);
	SAFE_DELETE(m_vecScene[SC_MAPEDIT]);

	m_pScene = CreateScene<StartScene>(SC_START);

	m_pPlayer = new Player;
	m_pPlayer->SetTag("Player");
	m_pPlayer->Init();

	GAME_MANAGER->SetPlayer(m_pPlayer);

	m_tNextState.nextScene = SC_NONE;
	m_tNextState.nextBeacon = BC_NONE;
	m_tNextState.nextDir = RIGHT;

	if (!GAME_MANAGER->Init())
	{
		throw EXCEPT(L"UI init failed.\n");
	}

	return true;
}

void SceneManager::Input(float dt)
{
	ChangeShowMode();
	if (KEYPRESS("TimeElapse"))
	{
		GAME_MANAGER->GameTimerFastForward();
	}
	m_pScene->Input(dt);
	if (m_bInGameScene)
	{
		GAME_MANAGER->Input(dt);
	}
}

int SceneManager::Update(float dt)
{
	m_pScene->Update(dt);
	if (m_bInGameScene)
	{
		GAME_MANAGER->Update(dt);
	}
	return m_iSignal;
}

int SceneManager::LateUpdate(float dt)
{
	m_pScene->LateUpdate(dt);
	if (m_bInGameScene)
	{
		GAME_MANAGER->LateUpdate(dt);
	}
	return m_iSignal;
}

void SceneManager::Collision(float dt)
{
	m_pScene->Collision(dt);
}

void SceneManager::Draw(HDC hdc, float dt)
{
	m_pScene->Draw(hdc, dt);
	DrawScreenDarkness(hdc);

	if (m_bInGameScene)
	{
		GAME_MANAGER->Draw(hdc, dt);
	}
		

	if (SHOWCHECK(SHOW_COLL))
	{
		COLLISION_MANAGER->Draw(hdc, dt);
	}
}

void SceneManager::ChangeScene()
{
	FadeOut();

	SCENE_CREATE cur = m_pScene->GetSceneType();
	switch (cur)
	{
	case SCENE_CREATE::SC_MAPEDIT:
		SAFE_DELETE(m_vecScene[cur]);
		break;
	}

	SCENE_CREATE nxt = m_tNextState.nextScene;
	if (!m_vecScene[nxt])
	{
		switch (nxt)
		{
		case SCENE_CREATE::SC_INHOUSE:
			CreateScene<InHouseScene>(nxt);
			GAME_MANAGER->Start();
			break;
		case SCENE_CREATE::SC_MAPEDIT:
			CreateScene<MapEditScene>(nxt);
			break;
		case SCENE_CREATE::SC_FARM:
			CreateScene<FarmScene>(nxt);
			break;
		case SCENE_CREATE::SC_MIDDLEWAY:
			CreateScene<MiddlewayScene>(nxt);
			break;
		case SCENE_CREATE::SC_TOWN:
			CreateScene<PelicantownScene>(nxt);
			break;
		case SCENE_CREATE::SC_STORE:
			CreateScene<TownStoreScene>(nxt);
			break;
		}
	}

	if (m_tNextState.bSleep)
	{
		GAME_MANAGER->SleepUntilMorning();
	}

	m_pScene = m_vecScene[nxt];

	m_bInGameScene = IsInGameScene();
	if (m_bInGameScene)
	{
		static_cast<GameScene*>(m_pScene)->SetUpScene(m_tNextState, m_pPlayer);
	}

	FadeIn();

	COLLISION_MANAGER->Clear();
	m_iSignal = 0;
	m_tNextState.bSleep = false;
	m_tNextState.nextBeacon = BC_NONE;
	m_tNextState.nextDir = LEFT;
	m_tNextState.nextScene = SC_NONE;
}

void SceneManager::ChangeShowMode()
{
	if (KEYDOWN("ShowOption"))
	{
		ToggleShowMode(SHOW_TILEOPTION);
	}
	if (KEYDOWN("ShowGrid"))
	{
		ToggleShowMode(SHOW_GRID);
	}
	if (KEYDOWN("ShowColl"))
	{
		ToggleShowMode(SHOW_COLL);
	}
}

void SceneManager::FadeOut()
{
	const int RSW = GETRESOLUTION.x;
	const int RSH = GETRESOLUTION.y;

	Texture* pEmptyTex = Texture::CreateEmptyTexture(WINDOW->GetWndDC(), RSW, RSH);

	m_fDelay = 0.f;
	RESOURCE_MANAGER->SetAlphaChannel(0);
	float th = 0.f;
	while (m_fDelay < m_fSceneDelay)
	{
		const float dt = TIMER->Tick();
		// FadeOut 장면 전환 효과
		m_fDelay += dt;
		if (m_fDelay > th)
		{
			th += m_fSceneDrawPeriod;
			int alpha = int(255.f * (m_fDelay / m_fSceneDelay));
			RESOURCE_MANAGER->SetAlphaChannel(alpha);
			AlphaBlend(WINDOW->GetWndDC(), 0, 0, RSW, RSH,
				pEmptyTex->GetDC(), 0, 0, RSW, RSH, RESOURCE_MANAGER->GetBlendFunc());
		}
	}

	SAFE_RELEASE(pEmptyTex);
}

void SceneManager::FadeIn()
{
	if (!m_pScene)
		return;

	// 카메라 조정
	CAMERA->Input(0.01f);
	const int RSW = GETRESOLUTION.x;
	const int RSH = GETRESOLUTION.y;

	Texture* pEmptyTex = Texture::CreateEmptyTexture(WINDOW->GetWndDC(), RSW, RSH);

	m_fDelay = 0.f;
	RESOURCE_MANAGER->SetAlphaChannel(0);
	float th = 0.f;
	while (m_fDelay < m_fSceneDelay)
	{
		const float dt = TIMER->Tick();
		// FadeOut 장면 전환 효과
		m_fDelay += dt;
		if (m_fDelay > th)
		{
			th += m_fSceneDrawPeriod;
			int alpha = int(255.f * (m_fDelay / m_fSceneDelay));
			RESOURCE_MANAGER->SetAlphaChannel(alpha);
			AlphaBlend(WINDOW->GetWndDC(), 0, 0, RSW, RSH,
				pEmptyTex->GetDC(), 0, 0, RSW, RSH, RESOURCE_MANAGER->GetBlendFunc());
		}
	}

	SAFE_RELEASE(pEmptyTex);
}

void SceneManager::DrawScreenDarkness(HDC hdc)
{
	if (!m_pScene)
		return;

	const int RSW = GETRESOLUTION.x;
	const int RSH = GETRESOLUTION.y;

	Texture* pEmptyTex = Texture::CreateEmptyTexture(WINDOW->GetWndDC(), RSW, RSH);

	float darkness = GAME_MANAGER->GetDayDarkNess();
	int alpha = int(255.f * darkness);
	RESOURCE_MANAGER->SetAlphaChannel(alpha);
	AlphaBlend(hdc, 0, 0, RSW, RSH,
		pEmptyTex->GetDC(), 0, 0, RSW, RSH, RESOURCE_MANAGER->GetBlendFunc());
	SAFE_RELEASE(pEmptyTex);
}

bool SceneManager::IsInGameScene() const
{
	if (m_pScene == nullptr)
		return false;

	return m_pScene->GetSceneType() != SC_START && m_pScene->GetSceneType() != SC_MAPEDIT;
}
void SceneManager::SignalizeSceneChange(const SceneState& state)
{
	m_iSignal = m_iChangeSignal;
	m_tNextState = state;
}

void SceneManager::PlayerSleep()
{
	SceneState state = {};
	state.bSleep = true;
	state.nextDir = LEFT;
	state.nextBeacon = BC_TWO;
	state.nextScene = SC_INHOUSE;
	SignalizeSceneChange(state);
}

