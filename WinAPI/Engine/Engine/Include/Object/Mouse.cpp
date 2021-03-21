#include "Mouse.h"
#include "../framework.h"
#include "../Application/Window.h"
#include "../Collider/ColliderPoint.h"
#include "../Core/Camera.h"

Mouse::Mouse()
    : m_tMove(), m_tWorldPos()
{
}

Mouse::Mouse(const Mouse& ui)
    : UI(ui), m_tMove(ui.m_tMove), m_tWorldPos(ui.m_tWorldPos)
{
}

Mouse::~Mouse()
{

}

bool Mouse::Init()
{
    POINT pt;
    // 스크린 좌표 기준으로 나온다.
    GetCursorPos(&pt);
    ScreenToClient(WINDOWHANDLE, &pt);

    ShowCursor(FALSE);

    m_tPos.x = (float) pt.x;
    m_tPos.y = (float) pt.y;

    ColliderPoint* pColl = AddCollider<ColliderPoint>("Mouse");
    pColl->SetUICollider(true);
    SAFE_RELEASE(pColl);

    return true;
}

void Mouse::Input(float dt)
{
    UI::Input(dt);
}

int Mouse::Update(float dt)
{
    UI::Update(dt);

    POINT pt;
    // 스크린 좌표 기준으로 나온다.
    GetCursorPos(&pt);
    ScreenToClient(WINDOWHANDLE, &pt);

    m_tMove.x = pt.x - m_tPos.x;
    m_tMove.y = pt.y - m_tPos.y;

    m_tPos.x = pt.x;
    m_tPos.y = pt.y;

    m_tWorldPos = m_tPos + CAMERA->GetTopLeft();
    return 0;
}

int Mouse::LateUpdate(float dt)
{
    UI::LateUpdate(dt);
    return 0;
}

void Mouse::Collision(float dt)
{
    UI::Collision(dt);
}

void Mouse::Draw(HDC hdc, float dt)
{
    UI::Draw(hdc, dt);
#ifdef _DEBUG
    Pos tPos = GetPos();
    wchar_t mousePos[32] = {};
    swprintf_s(mousePos, L"Mouse: %.1f, %.1f", GetWorldPos().x, GetWorldPos().y);
    TextOut(hdc, tPos.x, tPos.y, mousePos, lstrlen(mousePos));
#endif
}

Mouse* Mouse::Clone()
{
    return new Mouse(*this);
}
