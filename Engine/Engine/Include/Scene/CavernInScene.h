#pragma once
#include "GameScene.h"
class CavernInScene :
    public GameScene
{
private:
    friend class SceneManager;
private:
    const char* pGameDataFileName = "CavernIn.map";
private:
    CavernInScene();
    ~CavernInScene();
public:
    virtual bool Init();
    virtual void Input(float dt);
    virtual void Update(float dt);
    virtual void LateUpdate(float dt);
    virtual void Collision(float dt);
    virtual void Draw(HDC hdc, float dt);
};