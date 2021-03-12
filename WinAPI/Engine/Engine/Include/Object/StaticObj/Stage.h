#pragma once
#include "StaticObject.h"

class Stage : public StaticObject
{
	friend class MapEditScene;
	friend class Object;
	friend class Scene;
private:
	Stage();
	Stage(const Stage& obj);
	~Stage();
private:
	int m_iTileNumX = -1;
	int m_iTileNumY = -1;
	vector<class Tile*> m_baseTile;
public:
	const vector<class Tile*>* GetTiles() const { return &m_baseTile; }
	bool IsBlockTile(const Pos& worldPos) const;
	bool IsBlockTile(int index) const;
	void CreateTile(int iNumX, int iNumY);
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual Stage* Clone();
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
public:
	TILE_OPTION GetTileOption(const Pos& pos) const;
	string GetTileName(const Pos& pos) const;

	void SetTileNone(const Pos& tPos);
	void ChangeTileByCloneTile(const Pos& tPos, Tile* pClone);
	void ChangeTileOption(const Pos& tPos, TILE_OPTION eOption);
	inline int GetTileIndex(const Pos& tPos) const
	{
		return GetTileIndex(tPos.x, tPos.y);
	}
	inline int GetTileIndex(float x, float y) const
	{
		INDEX index = GetTileRowColIndex(x, y);

		if (index.x < 0 || index.x >= m_iTileNumX || index.y < 0 || index.y >= m_iTileNumY)
			return -1;

		return index.y * m_iTileNumX + index.x;
	}
	inline INDEX GetTileRowColIndex(const Pos& tPos) const
	{
		return GetTileRowColIndex(tPos.x, tPos.y);
	}
	inline INDEX GetTileRowColIndex(float x, float y) const
	{
		return INDEX(int(x) / TILESIZE, int(y) / TILESIZE);
	}
	inline Pos GetTilePos(int index)
	{
		int yIndex = index / m_iTileNumX;
		int xIndex = index % m_iTileNumX;

		return Pos(float(xIndex * TILESIZE), float(yIndex * TILESIZE));
	}
	inline Pos GetTilePos(const Pos& worldPos)
	{
		return GetTilePos(GetTileIndex(worldPos));
	}

	RESOLUTION GetTileSize() const { return { TILESIZE, TILESIZE }; }
	int GetStageWidth() const { return m_iTileNumX * TILESIZE; }
	int GetStageHeight() const { return m_iTileNumY * TILESIZE; }
	int GetStageTileNumX() const { return m_iTileNumX; }
	int GetStageTileNumY() const { return m_iTileNumY; }
private:
	void ClearTile();
};

