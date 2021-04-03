#include "CavernInScene.h"
#include "../Object/MoveObj/Player.h"
#include "../Object/StaticObj/GameManager.h"
#include "../Object/StaticObj/Stage.h"
#include "../Object/StaticObj/Tile.h"
#include "SceneManager.h"
CavernInScene::CavernInScene()
{
}

CavernInScene::~CavernInScene()
{
}

bool CavernInScene::Init()
{
	if (!GameScene::Init())
	{
		return false;
	}
	GameScene::SetUpScene(pGameDataFileName);
	return true;
}

void CavernInScene::Input(float dt)
{
	GameScene::Input(dt);
}

void CavernInScene::Update(float dt)
{
	GameScene::Update(dt);
}

void CavernInScene::LateUpdate(float dt)
{
	const Pos& curPos = AccessPlayer()->GetCenterPos();
	TILE_OPTION option = GetTileOption(curPos);
	switch (option)
	{
	case TO_BEACON_1:
		return;
	}

	GameScene::LateUpdate(dt);
}

void CavernInScene::Collision(float dt)
{
	GameScene::Collision(dt);
}

void CavernInScene::Draw(HDC hdc, float dt)
{
	GameScene::Draw(hdc, dt);
}