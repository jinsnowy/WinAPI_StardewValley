#include "InteractiveTile.h"
#include "../../Scene/Scene.h"

#include "../Item/Item.h"
InteractiveTile::InteractiveTile()
{
}

InteractiveTile::InteractiveTile(const InteractiveTile& obj)
    : Tile(obj)
{
    m_iHP = obj.m_iHP;
 
}

InteractiveTile::~InteractiveTile()
{
    SAFE_RELEASE(m_pItem);
}

void InteractiveTile::TileHit(Collider* pSrc, Collider* pDst, float dt)
{
}

void InteractiveTile::SetDropItem(Item* pItem)
{
    SAFE_RELEASE(m_pItem); 
    m_pItem = pItem; 
    if(m_pItem)
        m_pItem->AddRef();
}

void InteractiveTile::ItemDrop(int num)
{
    if (!m_pItem)
        return;

    Pos tPos = GetPos();
    for (int i = 0; i < num; ++i)
    {
        Item* pItem = m_pItem->Clone();
        
        pItem->SetPos(tPos);
        pItem->GenerateBoundEffect();

        m_pLayer->AddObject(pItem);

        SAFE_RELEASE(pItem);
    }
}

bool InteractiveTile::Init()
{
    return true;
}

void InteractiveTile::Input(float dt)
{
    Tile::Input(dt);
}

int InteractiveTile::Update(float dt)
{
    Tile::Update(dt);
    return 0;
}

int InteractiveTile::LateUpdate(float dt)
{
    Tile::LateUpdate(dt);
    return 0;
}

void InteractiveTile::Collision(float dt)
{
    Tile::Collision(dt);
}

void InteractiveTile::Draw(HDC hDC, float dt)
{
    Tile::Draw(hDC, dt);
}

void InteractiveTile::Save(FILE* pFile)
{
    Tile::Save(pFile);
}

void InteractiveTile::Load(FILE* pFile)
{
    Tile::Load(pFile);
}
