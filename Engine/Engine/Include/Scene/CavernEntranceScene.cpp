#include "CavernEntranceScene.h"
#include "../Object/MoveObj/Player.h"
#include "../Object/StaticObj/GameManager.h"
#include "../Object/StaticObj/Stage.h"
#include "../Object/StaticObj/Tile.h"
#include "SceneManager.h"
#include"../Sound/SoundManager.h"
#include "../Object/StaticObj/CropCabinet.h"
CavernEntranceScene::CavernEntranceScene()
{
}

CavernEntranceScene::~CavernEntranceScene()
{
}

bool CavernEntranceScene::Init()
{
	GameScene::SetUpScene(pGameDataFileName);

	return true;
}

void CavernEntranceScene::Input(float dt)
{
	GameScene::Input(dt);
}

void CavernEntranceScene::Update(float dt)
{
	GameScene::Update(dt);
}

void CavernEntranceScene::LateUpdate(float dt)
{
	const Pos& curPos = AccessPlayer()->GetCenterPos();
	TILE_OPTION option = GetTileOption(curPos);
	switch (option)
	{
	case TO_BEACON_1:
		SceneState state = {};
		state.nextDir = DOWN;
		state.nextBeacon = BC_THREE;
		state.nextScene = SC_FARM;

		SCENE_MANAGER->SignalizeSceneChange(state);
		return;
	}

	GameScene::LateUpdate(dt);
}

void CavernEntranceScene::Collision(float dt)
{
	GameScene::Collision(dt);
}

void CavernEntranceScene::Draw(HDC hdc, float dt)
{
	GameScene::Draw(hdc, dt);
}