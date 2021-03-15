#pragma once
#include "GameScene.h"

// 35 x 35
class FarmScene : public GameScene
{
private:
    friend class SceneManager;
private:
    const char* pGameDataFileName = "Farm.map";
    const Rect m_tMiddlewaySceneTrigger = {2232.f,880.f,2340.f,1060.f};
private:
    FarmScene();
    ~FarmScene();
public:
    virtual bool Init();
    virtual void Input(float dt);
    virtual void Update(float dt);
    virtual void LateUpdate(float dt);
    virtual void Collision(float dt);
    virtual void Draw(HDC hdc, float dt);
};