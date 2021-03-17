#include "GameScene.h"
#include "../Object/StaticObj/GameManager.h"
#include "../Object/StaticObj/Stage.h"
#include "../Resources/PrototypeManager.h"
#include "../Object/Object.h"
#include "../Core/PathManager.h"
#include "../Core/Input.h"
#include "../Core/Camera.h"
#include "../Object/MoveObj/Player.h"
#include "../Sound/SoundManager.h"
#include "../Object/StaticObj/Tile.h"
#include "../Object/StaticObj/Plant.h"
#include "../Object/Item/Seed.h"
#include "Layer.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
    SAFE_RELEASE(m_pGroundStage);
    SAFE_RELEASE(m_pStaticStage);
    SAFE_RELEASE(m_pPlayer);
}

Pos GameScene::FindBeacon(BEACON_TAG bc)
{
    TILE_OPTION option = TO_NONE;
    if (bc == BC_ONE)
        option = TO_BEACON_1;
    else if (bc == BC_TWO)
        option = TO_BEACON_2;
    else if (bc == BC_THREE)
        option = TO_BEACON_3;

    const auto tiles = m_pStaticStage->GetTiles();
    int found = -1;
    int size = (int) tiles->size();
    for (int i =0;i<size;++i)
    {
        if ((*tiles)[i]->GetTileOption() == option)
        {
            found = i;
            break;
        }
    }
    assert(found != -1 && option != TO_NONE);

    return m_pStaticStage->GetTilePos(found);
}

void GameScene::SetUpMainCharacter(Player* pPlayer)
{
    m_pPlayer = pPlayer;
    m_pPlayer->AddRef();

    AddObject(pPlayer);
    FindLayer("Object")->AddObject(pPlayer);

    m_pPlayer->LateInit();
}

void GameScene::SpawnMainCharacter(const Pos& worldPos)
{
    m_pPlayer->SetPos(worldPos);
}

int GameScene::GetTileIndex(const Pos& worldPos) const
{
    return m_pGroundStage->GetTileIndex(worldPos);
}

Pos GameScene::GetTilePos(const Pos& worldPos) const
{
    return m_pStaticStage->GetTilePos(worldPos);
}

INDEX GameScene::GetTileRowColIndex(const Pos& worldPos) const
{
    return m_pGroundStage->GetTileRowColIndex(worldPos);
}

INDEX GameScene::GetIndexDiff(const Pos& pos, const Pos& from) const
{
    return GetTileRowColIndex(pos) - GetTileRowColIndex(from);
}

bool GameScene::CheckValidIndex(INDEX index) const
{
    return m_pGroundStage->ValidIndex(index);
}

TILE_OPTION GameScene::GetTileOption(const Pos& worldPos) const
{
    return m_pStaticStage->GetTileOption(worldPos);
}


bool GameScene::IsBlockTile(const Pos& worldPos) const
{
    return m_pStaticStage->IsBlockTile(worldPos);
}

void GameScene::DigTile(const Pos& worldPos)
{
    // 농장 씬이 아닐 경우 (가정)
    if (GetSceneType() != SC_FARM)
        return;

    int index = GetTileIndex(worldPos);

    // 오브젝트가 타일에 있는 경우
    INDEX rcIndex = GetTileRowColIndex(worldPos);
    if (FindObjectByIndex(rcIndex))
        return;

    const string& texTag = m_pGroundStage->AccessTile(index)->GetTexTag();
    bool isDrought = texTag.starts_with("Dirt_Dig");
    bool isWet = texTag.starts_with("Dirt_Wet");
    if (texTag.starts_with("Dirt") && !isDrought && !isWet)
    {
        Tile* tile = m_pGroundStage->GetTile(index);
        tile->SetTexture("Dirt_Dig");
        SOUND_MANAGER->PlaySound("DirtDig");
        SAFE_RELEASE(tile);
    }
}

void GameScene::WaterTile(const Pos& worldPos)
{ 
    // 농장 씬이 아닐 경우 (가정)
    if (GetSceneType() != SC_FARM)
        return;

    int index = GetTileIndex(worldPos);

    const string& texTag = m_pGroundStage->AccessTile(index)->GetTexTag();
    bool isDrought = texTag.starts_with("Dirt_Dig");
    bool isWet = texTag.starts_with("Dirt_Wet");
    if (isDrought || isWet)
    {
        if (isDrought)
        {
            Tile* tile = m_pGroundStage->GetTile(index);
            tile->SetTexture("Dirt_Wet");
            SAFE_RELEASE(tile);
        }
        GAME_MANAGER->AddWateredTile(index);
        SOUND_MANAGER->PlaySound("WaterTile");
    }
}

void GameScene::SpawnPlant(const Pos& worldPos)
{
    // 농장 씬이 아닐 경우 (가정)
    if (GetSceneType() != SC_FARM)
        return;

    int index = GetTileIndex(worldPos);

    // 오브젝트가 타일에 있는 경우
    INDEX rcIndex = GetTileRowColIndex(worldPos);
    if (FindObjectByIndex(rcIndex))
        return;

    // 파져있는 r우
    const string& texTag = m_pGroundStage->AccessTile(index)->GetTexTag();
    bool isDrought = texTag.starts_with("Dirt_Dig");
    bool isWet = texTag.starts_with("Dirt_Wet");
    if (isDrought || isWet)
    {
        Seed* pSeed = static_cast<Seed*>(m_pPlayer->GetCurItem());

        Plant* pPlant = pSeed->SpawnPlant();
        if (pPlant)
        {
            pPlant->SetPosByIndex(rcIndex);
            pPlant->SetTileIndex(index);

            GAME_MANAGER->AddPlantList(pPlant);

            AddObject(pPlant, FindLayer("Object"));
            SAFE_RELEASE(pPlant);
        }

        pSeed->Decrease();
        // 씨앗을 다 썻을 경우
        if (!pSeed->GetLife())
        {
            m_pPlayer->EraseItem(pSeed);
        }
        SAFE_RELEASE(pSeed);
    }

}

bool GameScene::Init()
{
    return true;
}

void GameScene::Input(float dt)
{
    Scene::Input(dt);
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);
}

void GameScene::LateUpdate(float dt)
{
    Scene::LateUpdate(dt);
   
    Pos tPos = m_pPlayer->GetPos();
    float fScreenWidth = float(CAMERA->GetWorldRS().x);
    float fScreenHeight = float(CAMERA->GetWorldRS().y);

    if (m_pPlayer->GetLeft() < 0)
    {
        tPos.x -= m_pPlayer->GetLeft();
        m_pPlayer->SetPos(tPos);
    }
    else if (m_pPlayer->GetRight() >= fScreenWidth)
    {
        tPos.x -=  m_pPlayer->GetRight() - fScreenWidth + 1;
        m_pPlayer->SetPos(tPos);
    }
    else if (m_pPlayer->GetBottom() >= fScreenHeight)
    {
        tPos.y -=  m_pPlayer->GetBottom() - fScreenHeight + 1;
        m_pPlayer->SetPos(tPos);
    }
    else if (m_pPlayer->GetTop() < 0)
    {
        tPos.y -= m_pPlayer->GetTop();
        m_pPlayer->SetPos(tPos);
    }
}

void GameScene::Collision(float dt)
{
    Scene::Collision( dt);
}

void GameScene::Draw(HDC hdc, float dt)
{
    Scene::Draw(hdc, dt);

    Pos tPos = MOUSEWORLDPOS;
    int index = m_pGroundStage->GetTileIndex(tPos);
    Pos tilePos = m_pGroundStage->GetTilePos(index);
    tilePos -= CAMERA->GetTopLeft();
    DrawRedRect(hdc, MakeRect(int(tilePos.x), int(tilePos.y), TILESIZE, TILESIZE));

#ifdef _DEBUG
    stringstream ss;
    Pos playerPos = AccessPlayer()->GetPos();
    const string& tileOption = Scene::ConvertToNameOption(m_pStaticStage->GetTileOption(playerPos));
    const string& tileName = m_pGroundStage->GetTileName(playerPos);
    const string& stageName = m_pGroundStage->GetTag();
    const string& objName = Scene::GetNearObjectName(this, playerPos);
    ss << "[" << stageName << "]: " << tileName << ", " << tileOption;
    if (objName.size())
    {
        ss << " Object: " << objName;
    }
    ss << "\n";
    int length = (int) ss.str().size();
    playerPos -= CAMERA->GetTopLeft();
    TextOut(hdc, playerPos.x, playerPos.y + 10, GetWChar(ss.str().c_str()), length);
    ss.clear();
    ss.str("");
#endif
}

void GameScene::SetUpScene(const SceneState &state, Player* player)
{
    assert(player != nullptr);

    player->SetScene(this);
    player->SetLayer(FindLayer("Object"));

    if (m_pPlayer == nullptr)
    {
        SetUpMainCharacter(player);
    }

    if (state.nextBeacon != BC_NONE)
    {
        Pos tPos = FindBeacon(state.nextBeacon);
        switch (state.nextDir)
        {
        case LEFT:
            tPos.x -= TILESIZE;
            break;
        case RIGHT:
            tPos.x += TILESIZE;
            break;
        case UP:
            tPos.y -= TILESIZE;
            break;
        case DOWN:
            tPos.y += TILESIZE;
            break;
        }
        SpawnMainCharacter(tPos);
    }
    switch (state.nextDir)
    {
    case LEFT:
        m_pPlayer->StateTransit(Player::PlayerState::IDLE_LEFT);
        break;
    case RIGHT:
        m_pPlayer->StateTransit(Player::PlayerState::IDLE_RIGHT);
        break;
    case UP:
        m_pPlayer->StateTransit(Player::PlayerState::IDLE_RIGHT);
        break;
    case DOWN:
        m_pPlayer->StateTransit(Player::PlayerState::IDLE_DOWN);
        break;
    }

    int worldX = m_pStaticStage->GetStageWidth();
    int worldY = m_pStaticStage->GetStageHeight();

    CAMERA->SetWorldResolution(worldX, worldY);
    CAMERA->SetTarget(m_pPlayer);
    CAMERA->SetPivot(0.5f, 0.5f);
    CAMERA->SetPos(m_pPlayer->GetPos());
}

void GameScene::SetUpScene(const char* fileName)
{
    LoadDefaultStages(fileName);

    m_pGroundStage = static_cast<Stage*>(FindObject("GroundStage"));
    m_pGroundStage->AddRef();

    m_pStaticStage = static_cast<Stage*>(FindObject("StaticStage"));
    m_pStaticStage->AddRef();

    m_iTileNumX = m_pGroundStage->GetStageTileNumX();
    m_iTileNumY = m_pGroundStage->GetStageTileNumY();
}