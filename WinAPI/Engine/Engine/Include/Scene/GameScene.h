#pragma once
#include "Scene.h"
class GameScene : public Scene
{
    friend class SceneManager;
protected:
    class Stage* m_pGroundStage = nullptr;
    class Stage* m_pStaticStage = nullptr;
    class Player* m_pPlayer = nullptr;
    int m_iTileNumX = -1;
    int m_iTileNumY = -1;
protected:
    GameScene();
    ~GameScene() = 0;
public:
    const Player* AccessPlayer() const { return m_pPlayer; }
    TILE_OPTION GetTileOption(const Pos& worldPos) const;
    int GetTileIndex(const Pos& worldPos) const;
    Pos GetTilePos(const Pos& worldPos) const;
    INDEX GetTileRowColIndex(const Pos& worldPos) const;
    INDEX GetIndexDiff(const Pos& pos, const Pos& from) const;
    bool CheckValidIndex(INDEX index) const;
    bool IsBlockTile(const Pos& worldPos) const;
    void DigTile(const Pos& worldPos);
    void WaterTile(const Pos& worldPos);
    void SpawnPlant(const Pos& worldPos);
public:
    virtual void SetUpScene(const char* fileName);
    virtual bool Init();
    virtual void Input(float dt);
    virtual void Update(float dt);
    virtual void LateUpdate(float dt);
    virtual void Collision(float dt);
    virtual void Draw(HDC hdc, float dt);
private:
    Pos FindBeacon(BEACON_TAG bc);
    void SetUpScene(const SceneState& state, Player* player);
    void SetUpMainCharacter(Player* pPlayer);
    void SpawnMainCharacter(const Pos& worldPos);
};