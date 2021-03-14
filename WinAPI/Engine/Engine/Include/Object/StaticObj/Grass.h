#pragma once
#include "InteractiveTile.h"
class Grass : public InteractiveTile
{
	friend class PrototypeManager;
	friend class Object;
	friend class Stage;
private:
	static constexpr int m_iTypeNum = 3;
	static constexpr wchar_t m_strFolderPath[] = L"SV/Object/Outdoor/Grass/";
	static const wchar_t* const m_strBaseName[];
protected:
	Grass();
	Grass(const Grass& obj);
	virtual ~Grass();
	virtual void TileHit(Collider* pSrc, Collider* pDst, float dt);
	virtual void Die();
private:
	void ChangeTexture(int id);
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual Grass* Clone();
	virtual void LateInit();
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

