#include "Item.h"
unordered_map<string, Item*> Item::m_mapItem;

Item* Item::FindItem(const string& strItemKey)
{
    auto found = m_mapItem.find(strItemKey);
    if (found == m_mapItem.end())
        return nullptr;
    return found->second;
}

Item* Item::LoadItem(const string& strItemKey, const wchar_t* pFileName, const string& strPathKey)
{
    Item* pItem = FindItem(strItemKey);
    if (pItem)
        return pItem->Clone();

    pItem = Object::CreateObject<Item>(strItemKey);
    pItem->SetTexture(strItemKey, pFileName);
    pItem->SetAsTextureSize();
    pItem->SetColorKey(255, 255, 255);
    m_mapItem.insert(make_pair(strItemKey, pItem));

    return pItem->Clone();
}

Item::Item()
{
}

Item::Item(const Item& item)
    : Object(item)
{
    m_iItemNum = 1;
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

Item* Item::Clone()
{
    return new Item(*this);
}
