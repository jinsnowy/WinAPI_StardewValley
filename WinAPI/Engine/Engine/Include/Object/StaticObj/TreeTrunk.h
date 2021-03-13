#pragma once
#include "InteractiveTile.h"
class TreeTrunk : public InteractiveTile
{
	friend class PrototypeManager;
	friend class Object;
	friend class Scene;
private:
	static constexpr int m_iTypeNum = 1;
	static constexpr wchar_t m_strFolderPath[] = L"SV/Object/Outdoor/Tree/";
	static const wchar_t* const m_strBaseName[];
protected:
	TreeTrunk();
	TreeTrunk(const TreeTrunk& obj);
	virtual ~TreeTrunk();
	virtual void TileHit(Collider* pSrc, Collider* pDst, float dt);
	virtual void Die();

public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual TreeTrunk* Clone();
	virtual void LateInit();
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

