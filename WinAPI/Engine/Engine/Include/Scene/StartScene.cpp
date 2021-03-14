#include "StartScene.h"
#include "SceneManager.h"
#include "Layer.h"
#include "InHouseScene.h"
#include "../Application/Window.h"
#include "../Object/StaticObj/Stage.h"
#include "../Application/Window.h"
#include "../Core/Camera.h"
#include "../Object/StaticObj/UIPanel.h"
#include "../Object/StaticObj/UIButton.h"
#include "../Resources/ResourceManager.h"
#include "../Collider/ColliderRect.h"
#include "../Resources/Texture.h"
#include "MapEditScene.h"
#include "../Sound/SoundManager.h"


StartScene::StartScene()
{
}

StartScene::~StartScene()
{
}

bool StartScene::Init()
{
    if (!Scene::Init())
    {
        return false;
    }

    // 사운드 로드
    SOUND_MANAGER->LoadSound("StartScene_BGM", true, SD_BACKGROUND, "StartScene_BGM.mp3");
    SOUND_MANAGER->LoadSound("StartScene_Click", false, SD_EFFECT, "ClickButton_StartScene.mp3");
    SOUND_MANAGER->LoadSound("StartScene_On", false, SD_EFFECT, "ClickButtonOn_StartScene.mp3");

    Layer* pLayer = FindLayer("Ground");
    UIPanel* pBackPanel = CreateObject<UIPanel>("BackPanel", pLayer);

    pBackPanel->SetSize(GETRESOLUTION.x, GETRESOLUTION.y);
    pBackPanel->SetTexture("StartBack", L"StartScene/StardewValley_TitleScreen_BitMap.bmp");

    SAFE_RELEASE(pBackPanel);

    pLayer = FindLayer("UI");
    UIPanel* pGameTitle = CreateObject<UIPanel>("GameTitle", pLayer);
    pGameTitle->SetTexture("GameTitle", L"StartScene/NameTitle.bmp");
    pGameTitle->SetColorKey(255, 255, 255);
    pGameTitle->SetPos(GETRESOLUTION.x / 2.f, 200.f);
    pGameTitle->SetAsTextureSize();
    pGameTitle->SetPivot(0.5f, 0.5f);

    Texture* pTex = RESOURCE_MANAGER->LoadTexture("StartButton", L"StartScene/SelectScene.bmp");
    const float btnWidth = pTex->GetWidth() / 4.f;
    const float btnHeight = pTex->GetHeight() / 2.f;
    const float margin = 30.f;

    float stX = float(GETRESOLUTION.x / 2) - margin * 3 / 2.f - 2 * btnWidth;
    float stY = pGameTitle->GetPos().y + pGameTitle->GetSize().y / 2.f + 50.f;

    // 시작 버튼
    UIButton* pStartBtn = CreateObject<UIButton>("NewButton", pLayer);
    pStartBtn->SetPos(stX, stY);
    pStartBtn->SetSize(btnWidth, btnHeight);
    pStartBtn->SetTexture(pTex);
    pStartBtn->SetImageOffset(0.f, 0.f);
    pStartBtn->SetMouseOnOutImage(true);
    pStartBtn->SetMouseOutImageOffset(pStartBtn->GetImageOffset());
    pStartBtn->SetMouseOnImageOffset(pStartBtn->GetImageOffset().x,
                                    btnHeight + pStartBtn->GetImageOffset().y);
    pStartBtn->SetColorKey(255, 255, 255);
    pStartBtn->SetSoundTag("StartScene_On");

    ColliderRect* pRC = static_cast<ColliderRect*>(pStartBtn->GetCollider("ButtonBody"));
    pRC->SetRect(0.f, 0.f, btnWidth, btnHeight);
    SAFE_RELEASE(pRC);
    pStartBtn->SetCallback(this, &StartScene::StartButtonCallback);
    SAFE_RELEASE(pStartBtn);

    // 편집 버튼
    stX += margin + btnWidth;
    UIButton* pEditBtn = CreateObject<UIButton>("EditButton", pLayer);
    pEditBtn->SetPos(stX, stY);
    pEditBtn->SetSize(btnWidth, btnHeight);
    pEditBtn->SetTexture(pTex);
    pEditBtn->SetImageOffset(btnWidth, 0.f);
    pEditBtn->SetMouseOnOutImage(true);
    pEditBtn->SetMouseOutImageOffset(pEditBtn->GetImageOffset());
    pEditBtn->SetMouseOnImageOffset(pEditBtn->GetImageOffset().x,
                                    btnHeight + pEditBtn->GetImageOffset().y);
    pEditBtn->SetColorKey(255, 255, 255);
    pEditBtn->SetSoundTag("StartScene_On");

    pRC = static_cast<ColliderRect*>(pEditBtn->GetCollider("ButtonBody"));
    pRC->SetRect(0.f, 0.f, btnWidth, btnHeight);
    SAFE_RELEASE(pRC);
    pEditBtn->SetCallback(this, &StartScene::EditButtonCallback);
    SAFE_RELEASE(pEditBtn);

    // Op
    stX += margin + btnWidth;
    UIButton* pOpBtn = CreateObject<UIButton>("NoneButton", pLayer);
    pOpBtn->SetPos(stX, stY);
    pOpBtn->SetSize(btnWidth, btnHeight);
    pOpBtn->SetTexture(pTex);
    pOpBtn->SetImageOffset(2 * btnWidth, 0.f);
    pOpBtn->SetMouseOnOutImage(true);
    pOpBtn->SetMouseOutImageOffset(pOpBtn->GetImageOffset());
    pOpBtn->SetMouseOnImageOffset(pOpBtn->GetImageOffset().x,
        btnHeight + pOpBtn->GetImageOffset().y);
    pOpBtn->SetColorKey(255, 255, 255);
    pOpBtn->SetSoundTag("StartScene_On");
    SAFE_RELEASE(pOpBtn);

    // 종료 버튼
    stX += margin + btnWidth;
    UIButton* pEndBtn = CreateObject<UIButton>("ExitButton", pLayer);
    pEndBtn->SetPos(stX, stY);
    pEndBtn->SetSize(btnWidth, btnHeight);
    pEndBtn->SetTexture(pTex);
    pEndBtn->SetImageOffset(3 * btnWidth, 0.f);
    pEndBtn->SetMouseOnOutImage(true);
    pEndBtn->SetMouseOutImageOffset(pEndBtn->GetImageOffset());
    pEndBtn->SetMouseOnImageOffset(pEndBtn->GetImageOffset().x,
                                    btnHeight + pEndBtn->GetImageOffset().y);
    pEndBtn->SetColorKey(255, 255, 255);
    pEndBtn->SetSoundTag("StartScene_On");

    pRC = static_cast<ColliderRect*>(pEndBtn->GetCollider("ButtonBody"));
    pRC->SetRect(0.f, 0.f, btnWidth, btnHeight);
    SAFE_RELEASE(pRC);
    pEndBtn->SetCallback(this, &StartScene::EndButtonCallback);
    SAFE_RELEASE(pEndBtn);

    SAFE_RELEASE(pGameTitle);
    SAFE_RELEASE(pTex);
    SOUND_MANAGER->PlaySound("StartScene_BGM");
	return true;
}

void StartScene::StartButtonCallback(float fTime)
{
    SOUND_MANAGER->PlaySound("StartScene_Click");
    SOUND_MANAGER->PauseSound(SD_BACKGROUND);

    SceneState nxt = {};
    nxt.nextDir = LEFT;
    nxt.nextBeacon = BC_TWO;
    nxt.nextScene = SC_INHOUSE;
    SCENE_MANAGER->SignalizeSceneChange(nxt);
}

void StartScene::EndButtonCallback(float fTime)
{
    SOUND_MANAGER->PlaySound("StartScene_Click");
    SOUND_MANAGER->PauseSound(SD_BACKGROUND);
    WINDOW->DestroyGame();
}

void StartScene::EditButtonCallback(float fTime)
{
    SceneState nxt = {};
    nxt.nextDir = RIGHT;
    nxt.nextBeacon = BC_NONE;
    nxt.nextScene = SC_MAPEDIT;
    SOUND_MANAGER->PlaySound("StartScene_Click");
    SOUND_MANAGER->PauseSound(SD_BACKGROUND);
    SCENE_MANAGER->SignalizeSceneChange(nxt);
}
