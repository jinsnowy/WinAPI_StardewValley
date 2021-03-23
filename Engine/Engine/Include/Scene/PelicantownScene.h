#pragma once
#include "GameScene.h"

// 35 x 20
class PelicantownScene : public GameScene
{
private:
    friend class SceneManager;
private:
    const char* pGameDataFileName = "PelicanTown.map";
    const Rect m_tMiddlewayTrigger = {-64.f, 320.f,0.f,632.f};
private:
    PelicantownScene();
    ~PelicantownScene();
public:
    virtual bool Init();
    virtual void Input(float dt);
    virtual void Update(float dt);
    virtual void LateUpdate(float dt);
    virtual void Collision(float dt);
    virtual void Draw(HDC hdc, float dt);
};