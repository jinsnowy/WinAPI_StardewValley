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
	enum class State
	{
		OPEN,
		CLOSE,
	};
	State m_State = State::CLOSE;
	static constexpr float m_fPosX = 1600.f;
	static constexpr float m_fPosY = 780.f;
	static constexpr float m_iWidth = 18.f;
	static constexpr float m_iHeight = 36.f;
	void Click(class Collider* pSrc, class Collider* pDst, float dt);
	void OnSite(class Collider* pSrc, class Collider* pDst, float dt);
	void OffSite(class Collider* pSrc, class Collider* pDst, float dt);
	void SoldEffect(class Item* pItem);
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual void LateInit();
};