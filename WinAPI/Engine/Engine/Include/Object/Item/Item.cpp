#include "Item.h"

Item::Item()
{
}

Item::Item(const Item& item)
    : Object(item)
{
    m_iItemNum = item.m_iItemNum;
}

Item::~Item()
{
}

bool Item::Init()
{
    return true;
}

void Item::Input(float dt)
{
    Object::Input(dt);
}

int Item::Update(float dt)
{
    Object::Update(dt);
    return 0;
}

int Item::LateUpdate(float dt)
{
    Object::LateUpdate(dt);
    return 0;
}

void Item::Collision(float dt)
{
    Object::Collision(dt);
}

void Item::Draw(HDC hdc, float dt)
{
    Object::Draw(hdc, dt);
}
