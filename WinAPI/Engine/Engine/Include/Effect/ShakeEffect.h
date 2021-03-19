#pragma once
#include "Effect.h"
class ShakeEffect : public Effect
{
private:
	float m_fShakeTime = 0.f;
	float m_fShakePeriod;
	float m_fRangeX;
	float m_fRangeY;
	float m_fDampingCoeff;
	float m_fNoiseCoeff;
	float m_fAlpha = 1.0f;
	static normal_distribution<float> m_noiseDist;
protected:
	virtual bool Predicate();
	virtual void Process(float dt);
public:
	explicit ShakeEffect(class Object* pObj, float duration, float fPeriod, float fRangeX, float fRangeY, float m_fNoiseCoeff = 1.0f, float dampingCoeff = 0.96f);
	~ShakeEffect();
};