#pragma once
#include "InteractiveTile.h"
class Rock : public InteractiveTile
{
	friend class Object;
private:
	static constexpr int m_iTypeNum = 3;
	static constexpr wchar_t m_strFolderPath[] = L"SV/Object/Outdoor/Rock/";
	static const wchar_t* const m_strBaseName[];
protected:
	Rock();
	Rock(const Rock& obj);
	virtual ~Rock();
	virtual void TileHit(Collider* pSrc, Collider* pDst, float dt);
	virtual void Die();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual Rock* Clone();
	virtual void LateInit();
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	void ChangeTexture(int id);
};

