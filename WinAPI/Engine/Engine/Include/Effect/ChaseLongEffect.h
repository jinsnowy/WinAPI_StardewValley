#pragma once
#include "Effect.h"
class ChaseLongEffect :
    public Effect
{
private:
	class Object* m_pTarget;
	static constexpr float m_fChaseRange = 400.f;
	static constexpr float m_fChaseSpeed = 350.f;
	static constexpr float m_fSinusoidalPeriod = 0.2f;
	static constexpr float m_fGain = 8.f;
	static constexpr float W = 2 * PI / m_fSinusoidalPeriod;
	float m_fDist = FLT_MAX;
protected:
	virtual void Process(float dt);
	virtual bool Predicate() const;
public:
	explicit ChaseLongEffect(Object* pSubject, Object* pTarget);
	~ChaseLongEffect();
};
