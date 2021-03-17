#include "Tool.h"

Tool::Tool()
{
    m_eItemType = ITEM_TOOL;
}

Tool::Tool(const Tool& item)
    : Item(item)
{
    m_iPower = item.m_iPower;
    m_iEndure = item.m_iEndure;
}

Tool::~Tool()
{
}

bool Tool::Init()
{
    return true;
}

void Tool::Input(float dt)
{
}

int Tool::Update(float dt)
{
    return 0;
}

int Tool::LateUpdate(float dt)
{
    return 0;
}

void Tool::Collision(float dt)
{
}

void Tool::Draw(HDC hdc, float dt)
{
}

Tool* Tool::Clone()
{
    return new Tool(*this);
}
