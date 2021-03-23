#pragma once
#include "GameScene.h"
// 50 x50
class InHouseScene : public GameScene
{
private:
    friend class SceneManager;
private:
    bool m_bClick = false;
    bool m_bPlayerOut = false;
    bool m_bPlayerOnceOut = true;
    Rect m_tSelect = {};
    const char*  pGameDataFileName = "InHouse.map";
    class UIPanel* m_pSleepPanel = nullptr;
    const Rect m_tSleepTrigger = { 1224.f, 762.f, 1275.f, 930.f };
private:
    InHouseScene();
    ~InHouseScene();
public:
    virtual void SetUpCamera();
    virtual bool Init();
    virtual void Input(float dt);
    virtual void Update(float dt);
    virtual void LateUpdate(float dt);
    virtual void Collision(float dt);
    virtual void Draw(HDC hdc, float dt);
private:
    void SleepCallBack(Collider* pSrc, Collider* pDst, float dt);
    void CancelCallBack(Collider* pSrc, Collider* pDst, float dt);
};