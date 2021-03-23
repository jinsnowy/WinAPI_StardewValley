#include "MiddlewayScene.h"
#include "../Object/MoveObj/Player.h"
#include "../Sound/SoundManager.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"
#include "SceneManager.h"


MiddlewayScene::MiddlewayScene()
{
}

MiddlewayScene::~MiddlewayScene()
{
}

bool MiddlewayScene::Init()
{
	if (!GameScene::Init())
	{
		return false;
	}

	GameScene::SetUpScene(pGameDataFileName);

	return true;
}

void MiddlewayScene::Input(float dt)
{
	GameScene::Input(dt);
}

void MiddlewayScene::Update(float dt)
{
	GameScene::Update(dt);
}

void MiddlewayScene::LateUpdate(float dt)
{
	if (m_tFarmSceneTrigger.IsCollideRect(AccessPlayer()->GetBodyRect()))
	{
		SceneState state = {};
		state.nextDir = LEFT;
		state.nextBeacon = BC_TWO;
		state.nextScene = SC_FARM;
		SCENE_MANAGER->SignalizeSceneChange(state);
		return;
	}
	if (m_tTownSceneTrigger.IsCollideRect(AccessPlayer()->GetBodyRect()))
	{
		SceneState state = {};
		state.nextDir = RIGHT;
		state.nextBeacon = BC_TWO;
		state.nextScene = SC_TOWN;
		SCENE_MANAGER->SignalizeSceneChange(state);
		return;
	}

	GameScene::LateUpdate(dt);
}

void MiddlewayScene::Collision(float dt)
{
	GameScene::Collision(dt);
}

void MiddlewayScene::Draw(HDC hdc, float dt)
{
	GameScene::Draw(hdc, dt);
}
