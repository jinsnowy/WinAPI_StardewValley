#pragma once
#include "Effect.h"
class BoundEffect : public Effect
{
private:
	static constexpr float m_fCoeff = 0.7f;
	int m_iBounceCount = 0;
	int m_iMaxBounceNum = 5;
	float m_fVeloY = 0.f;
	float m_fVeloX = 0.f;
	float m_fYLimit = 0.f;
protected:
	virtual void Process(float dt);
	virtual bool Predicate() const;
public:
	explicit BoundEffect(const Pos& origin, float duration, int maxBounce,
							float fAngle, float fVelo, float YLimit);
	~BoundEffect();
};