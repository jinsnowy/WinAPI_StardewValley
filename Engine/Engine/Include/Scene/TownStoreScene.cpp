#include "TownStoreScene.h"
#include "../Object/MoveObj/Player.h"
#include "../Sound/SoundManager.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"
#include "SceneManager.h"

TownStoreScene::TownStoreScene()
{
}

TownStoreScene::~TownStoreScene()
{
}

bool TownStoreScene::Init()
{
	if (!GameScene::Init())
	{
		return false;
	}
    GameScene::SetUpScene(pGameDataFileName);
    return true;
}

void TownStoreScene::Input(float dt)
{
    GameScene::Input(dt);
}

void TownStoreScene::Update(float dt)
{
    GameScene::Update(dt);
}

void TownStoreScene::LateUpdate(float dt)
{
	const Pos& curPos = AccessPlayer()->GetCenterPos();
	TILE_OPTION option = GetTileOption(curPos);
	switch (option)
	{
	case TO_BEACON_1:
		SceneState state = {};
		state.nextDir = DOWN;
		state.nextBeacon = BC_ONE;
		state.nextScene = SC_TOWN;
		SCENE_MANAGER->SignalizeSceneChange(state);
		return;
	}
    GameScene::LateUpdate(dt);
}

void TownStoreScene::Collision(float dt)
{
    GameScene::Collision(dt);
}

void TownStoreScene::Draw(HDC hdc, float dt)
{
    GameScene::Draw(hdc, dt);
}

