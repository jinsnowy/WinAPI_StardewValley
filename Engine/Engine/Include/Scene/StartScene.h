#pragma once
#include "Scene.h"
class StartScene :
    public Scene
{
private:
    friend class SceneManager;
private:
    StartScene();
    ~StartScene();
public:
    virtual bool Init();

public:
    void StartButtonCallback(float fTime);
    void EndButtonCallback(float fTime);
    void EditButtonCallback(float fTime);
};