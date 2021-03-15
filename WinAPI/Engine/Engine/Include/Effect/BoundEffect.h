#pragma once
#include "../framework.h"
class BoundEffect
{
private:
	bool m_bEnd = false;
	int m_iBounceCount = 0;
	int m_iMaxBounceNum = 5;
	float m_fVeloY = 0.f;
	float m_fVeloX = 0.f;
	float m_fCoeff = 0.7f;
	Pos m_tPos = Pos(0, 0);
	float m_fYLimit = 0.f;
	float m_fDeltaTime = 0.f;
	float m_fDuration = 3.f;
public:
	BoundEffect();
	~BoundEffect();
	void Init(const Pos& origin, int maxBounce, 
		float fAngle, float fVelo, float YLimit, float duration);
	bool IsEnd()const { return m_bEnd; }
	Pos Next(float dt);
};