#pragma once
#include "GameScene.h"
// 20 x 30
class TownStoreScene : public GameScene
{
private:
    friend class SceneManager;
private:
    const char* pGameDataFileName = "TownStore.map";
private:
    TownStoreScene();
    ~TownStoreScene();
public:
    virtual bool Init();
    virtual void Input(float dt);
    virtual void Update(float dt);
    virtual void LateUpdate(float dt);
    virtual void Collision(float dt);
    virtual void Draw(HDC hdc, float dt);
};