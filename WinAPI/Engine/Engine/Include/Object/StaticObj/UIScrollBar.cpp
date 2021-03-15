#include "UIScrollBar.h"

UIScrollBar::UIScrollBar()
{
}

UIScrollBar::UIScrollBar(const UIScrollBar& ui)
    : UI (ui)
{
}

UIScrollBar::~UIScrollBar()
{
}

bool UIScrollBar::Init()
{
    return true;
}

void UIScrollBar::Input(float dt)
{
    UI::Input(dt);
}

int UIScrollBar::Update(float dt)
{
    UI::Update(dt);
    return 0;
}

int UIScrollBar::LateUpdate(float dt)
{
    UI::LateUpdate(dt);
    return 0;
}

void UIScrollBar::Collision(float dt)
{
    UI::Collision(dt);
}

void UIScrollBar::Draw(HDC hdc, float dt)
{
    UI::Draw(hdc, dt);
}

UIScrollBar* UIScrollBar::Clone()
{
    return new UIScrollBar(*this);
}
