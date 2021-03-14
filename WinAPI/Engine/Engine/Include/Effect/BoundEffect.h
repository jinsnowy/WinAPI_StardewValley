#pragma once
#include "../framework.h"
class BoundEffect
{
private:
	bool m_bEnd = false;
	int m_iBounceCount = 0;
	int m_iMaxBounceNum;
	float m_fVeloY;
	float m_fVeloX;
	float m_fCoeff = 0.7f;
	Pos m_tPos;
	float m_fYLimit;
	float m_fDeltaTime = 0.f;
	float m_fDuration;
public:
	BoundEffect();
	~BoundEffect();
	void Init(const Pos& origin, int maxBounce, 
		float fAngle, float fVelo, float YLimit, float duration);
	bool IsEnd()const { return m_bEnd; }
	Pos Next(float dt);
};