#include "InteractiveTile.h"

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
}

void InteractiveTile::TileHit(Collider* pSrc, Collider* pDst, float dt)
{
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
