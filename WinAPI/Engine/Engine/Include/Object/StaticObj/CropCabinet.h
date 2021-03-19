#pragma once
#include "StaticObject.h"

class CropCabinet :
	public StaticObject
{
	friend class Object;
protected:
	CropCabinet();
	CropCabinet(const CropCabinet& obj);
	virtual ~CropCabinet();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual CropCabinet* Clone();
private:
	static constexpr float m_fPosX = 1600.f;
	static constexpr float m_fPosY = 820.f;
	void Click(class Collider* pSrc, class Collider* pDst, float dt);
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual void LateInit();
};