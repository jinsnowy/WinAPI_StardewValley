#include "TileObject.h"

TileObject::TileObject()
    :
    m_iTileIndex(-1)
{
}

TileObject::TileObject(const TileObject& obj)
    :Tile(obj)
{
    m_iTileIndex = obj.m_iTileIndex;
}

TileObject::~TileObject()
{
}

bool TileObject::Init()
{
    return true;
}

void TileObject::Input(float dt)
{
    Tile::Input(dt);
}

int TileObject::Update(float dt)
{
    Tile::Update(dt);
    return 0;
}

int TileObject::LateUpdate(float dt)
{
    Tile::LateUpdate(dt);
    return 0;
}

void TileObject::Collision(float dt)
{
    Tile::Collision(dt);
}

void TileObject::Draw(HDC hDC, float dt)
{
    Tile::Draw(hDC, dt);
}

TileObject* TileObject::Clone()
{
    return new TileObject(*this);
}

void TileObject::Save(FILE* pFile)
{
    Tile::Save(pFile);

    fwrite(&m_iTileIndex, 4, 1, pFile);
}

void TileObject::Load(FILE* pFile)
{
    Tile::Load(pFile);

    fread(&m_iTileIndex, 4, 1, pFile);
}
