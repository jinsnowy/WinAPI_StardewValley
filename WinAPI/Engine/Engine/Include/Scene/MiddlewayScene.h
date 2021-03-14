#pragma once
#include "GameScene.h"
class MiddlewayScene : public GameScene
{
private:
    friend class SceneManager;
private:
    const char* pGameDataFileName = "Middleway.map";
    const Rect m_tFarmSceneTrigger = { -64.f, 1792.f,16.f,2048.f };
private:
    MiddlewayScene();
    ~MiddlewayScene();
public:
    virtual bool Init();
    virtual void Input(float dt);
    virtual void Update(float dt);
    virtual void LateUpdate(float dt);
    virtual void Collision(float dt);
    virtual void Draw(HDC hdc, float dt);
};