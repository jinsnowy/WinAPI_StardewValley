#include "CavernEntranceScene.h"
#include "../Object/MoveObj/Player.h"
#include "../Object/StaticObj/GameManager.h"
#include "../Object/StaticObj/Stage.h"
#include "../Object/StaticObj/Tile.h"
#include "SceneManager.h"
#include"../Sound/SoundManager.h"
#include "../Object/StaticObj/CropCabinet.h"
#include "../Collider/ColliderRect.h"
#include "../Object/PointObject.h"
CavernEntranceScene::CavernEntranceScene()
{
}

CavernEntranceScene::~CavernEntranceScene()
{
	SAFE_RELEASE(m_pLadder);
}

void CavernEntranceScene::LadderDown(Collider* pSrc, Collider* pDst, float dt)
{
	if (pDst->GetTag() == "Click")
	{
		SceneState state = {};
		state.nextDir = DOWN;
		state.nextBeacon = BC_THREE;
		state.nextScene = SC_CAVERN_IN;

		SCENE_MANAGER->SignalizeSceneChange(state);
	}
}

bool CavernEntranceScene::Init()
{
	if (!GameScene::Init())
	{
		return false;
	}
	GameScene::SetUpScene(pGameDataFileName);
	m_pLadder = CreateObject<PointObject>("Ladder", FindLayer("Object"));
	m_pLadder->SetPos(576.f, 374.f);
	m_pLadder->SetSize(64.f, 64.f);
	ColliderRect* pColl = m_pLadder->AddCollider<ColliderRect>("LadderTrigger");
	pColl->AddCollisionFunction(CS_ENTER, this, &CavernEntranceScene::LadderDown);
	pColl->SetRect(0.f, 0.f, 64.f, 64.f);
	SAFE_RELEASE(pColl);
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