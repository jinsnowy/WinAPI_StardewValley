#include "UIScrollBar.h"
#include "../../Core/Input.h"
#include "UIButton.h"
#include "../../Collider/ColliderRect.h"
#include "../../Collider/CollisionManager.h"
UIScrollBar::UIScrollBar()
{
}

UIScrollBar::UIScrollBar(const UIScrollBar& ui)
    : UI (ui)
{
}

UIScrollBar::~UIScrollBar()
{
    SAFE_RELEASE(m_pScroller);
}

bool UIScrollBar::Init()
{
    m_pScroller = Object::CreateObject<UIButton>("Scroller");
    return true;
}

void UIScrollBar::Input(float dt)
{
    UI::Input(dt);

    int mouseDelta = INPUT->GetMouseScroll();
    if (mouseDelta != 0)
    {
        m_iCurBlock -= mouseDelta;
        m_iCurBlock = max(min(m_iCurBlock, m_iNumBlock - 1), 0);

        Pos tPos = GetPos();
        int hBlock = GetSize().y / m_iNumBlock;
        m_pScroller->SetPos(tPos.x, tPos.y + hBlock * m_iCurBlock);
    }
}

int UIScrollBar::Update(float dt)
{
    UI::Update(dt);
    m_pScroller->Update(dt);
    m_pScroller->LateUpdate(dt);
    COLLISION_MANAGER->AddObject(m_pScroller);
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
    m_pScroller->Draw(hdc, dt);
}

UIScrollBar* UIScrollBar::Clone()
{
    return new UIScrollBar(*this);
}

UIButton* UIScrollBar::GetScroller()
{
    m_pScroller->AddRef();
    return m_pScroller;
}
