#include "PelicantownScene.h"
#include "../Object/MoveObj/Player.h"
#include "../Sound/SoundManager.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"
#include "SceneManager.h"

PelicantownScene::PelicantownScene()
{
}

PelicantownScene::~PelicantownScene()
{
}

bool PelicantownScene::Init()
{
	GameScene::SetUpScene(pGameDataFileName);
	return true;
}

void PelicantownScene::Input(float dt)
{
	GameScene::Input(dt);
}

void PelicantownScene::Update(float dt)
{
	GameScene::Update(dt);
}

void PelicantownScene::LateUpdate(float dt)
{
	const Pos& curPos = AccessPlayer()->GetCenterPos();
	TILE_OPTION option = GetTileOption(curPos);
	switch (option)
	{
	case TO_BEACON_1:
		break;
	}

	if (m_tMiddlewayTrigger.IsCollideRect(AccessPlayer()->GetBodyRect()))
	{
		SceneState state = {};
		state.nextDir = LEFT;
		state.nextBeacon = BC_ONE;
		state.nextScene = SC_MIDDLEWAY;
		SCENE_MANAGER->SignalizeSceneChange(state);
	}
	GameScene::LateUpdate(dt);
}

void PelicantownScene::Collision(float dt)
{
	GameScene::Collision(dt);
}

void PelicantownScene::Draw(HDC hdc, float dt)
{
	GameScene::Draw(hdc, dt);
}
