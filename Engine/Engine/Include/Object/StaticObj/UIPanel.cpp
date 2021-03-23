#include "UIPanel.h"

UIPanel::UIPanel()
{
}

UIPanel::UIPanel(const UIPanel& ui)
    : UI (ui)
{
}

UIPanel::~UIPanel()
{
}

bool UIPanel::Init()
{
    return true;
}

void UIPanel::Input(float dt)
{
    UI::Input(dt);
}

int UIPanel::Update(float dt)
{
    UI::Update(dt);
    return 0;
}

int UIPanel::LateUpdate(float dt)
{
    UI::LateUpdate(dt);
    return 0;
}

void UIPanel::Collision(float dt)
{
    UI::Collision(dt);
}

void UIPanel::Draw(HDC hdc, float dt)
{
    UI::Draw(hdc, dt);
}

UIPanel* UIPanel::Clone()
{
    return new UIPanel(*this);
}
