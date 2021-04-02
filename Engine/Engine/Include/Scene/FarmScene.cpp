#include "FarmScene.h"
#include "../Object/MoveObj/Player.h"
#include "../Object/StaticObj/GameManager.h"
#include "../Object/StaticObj/Stage.h"
#include "../Object/StaticObj/Tile.h"
#include "SceneManager.h"
#include"../Sound/SoundManager.h"
#include "../Object/StaticObj/CropCabinet.h"
FarmScene::FarmScene()
{
}

FarmScene::~FarmScene()
{
}

bool FarmScene::Init()
{
	GameScene::SetUpScene(pGameDataFileName);

	SOUND_MANAGER->LoadSound("DoorOpen", false, SD_EFFECT, "DoorOpen.mp3");
	SOUND_MANAGER->LoadSound("ItemGet", false, SD_EFFECT, "ItemGet.mp3");
	SOUND_MANAGER->LoadSound("TreeHit", false, SD_EFFECT, "Object/TreeHit.mp3");
	SOUND_MANAGER->LoadSound("TreeOver", false, SD_EFFECT, "Object/TreeOver.mp3");
	SOUND_MANAGER->LoadSound("RockOver", false, SD_EFFECT, "Object/RockOver.mp3");
	SOUND_MANAGER->LoadSound("GrassOver", false, SD_EFFECT, "Object/GrassOver.mp3");
	SOUND_MANAGER->LoadSound("DirtDig", false, SD_EFFECT, "Object/DirtDig.mp3");
	SOUND_MANAGER->LoadSound("WaterTile", false, SD_EFFECT, "Object/WaterTile.mp3");

	Object* pObj = CreateObject<CropCabinet>("CropCabinet", FindLayer("Object"));
	SAFE_RELEASE(pObj);
	return true;
}

void FarmScene::Input(float dt)
{
	GameScene::Input(dt);

	vector<int> droughtTileIndices = GAME_MANAGER->GetDroughtTiles();
	for (int index : droughtTileIndices)
	{
		Tile* pTile = m_pGroundStage->GetTile(index);
		pTile->SetTexture("Dirt_Dig");
		SAFE_RELEASE(pTile);
	}
}

void FarmScene::Update(float dt)
{
	GameScene::Update(dt);
}

void FarmScene::LateUpdate(float dt)
{
	const Pos& curPos = AccessPlayer()->GetCenterPos();
	TILE_OPTION option = GetTileOption(curPos);
	SceneState state = {};
	switch (option)
	{
	case TO_BEACON_1:
		state.nextDir = UP;
		state.nextBeacon = BC_ONE;
		state.nextScene = SC_INHOUSE;
		SOUND_MANAGER->PlayMusic("DoorOpen");
		SCENE_MANAGER->SignalizeSceneChange(state);
		return;
	case TO_BEACON_3:
		state.nextDir = UP;
		state.nextBeacon = BC_ONE;
		state.nextScene = SC_CAVERN_ENTRANCE;
		SCENE_MANAGER->SignalizeSceneChange(state);
		return;
	}

	if (m_tMiddlewaySceneTrigger.IsCollideRect(AccessPlayer()->GetBodyRect()))
	{
		state.nextDir = RIGHT;
		state.nextBeacon = BC_TWO;
		state.nextScene = SC_MIDDLEWAY;
		SCENE_MANAGER->SignalizeSceneChange(state);
		return;
	}

	GameScene::LateUpdate(dt);
}

void FarmScene::Collision(float dt)
{
	GameScene::Collision(dt);
}

void FarmScene::Draw(HDC hdc, float dt)
{
	GameScene::Draw(hdc, dt);
}