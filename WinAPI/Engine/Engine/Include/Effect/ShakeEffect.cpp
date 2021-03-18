#include "ShakeEffect.h"

normal_distribution<float> ShakeEffect::m_noiseDist(0.f, 0.5f);

ShakeEffect::ShakeEffect(const Pos& origin, float duration, 
	float fPeriod, float fRangeX, float fRangeY, float fNoiseCoeff, float fDampingCoeff)
	: Effect(origin, duration)
{
	m_fRangeX = max(fRangeX, 0.0f);
	m_fRangeY = max(fRangeY, 0.0f);

	m_tOrigin = origin;
	m_fShakePeriod = fPeriod;
	m_fNoiseCoeff = fNoiseCoeff;
	m_fDampingCoeff = fDampingCoeff;
}

ShakeEffect::~ShakeEffect()
{
}

bool ShakeEffect::Predicate()
{
	return m_fAlpha < 0.01f;
}

void ShakeEffect::Process(float dt)
{
	if(m_fDuration - m_fShakeTime >= m_fShakePeriod)
	{
		m_fShakeTime = m_fDuration;

		float dx = m_fAlpha * m_fRangeX;
		float dy = m_fAlpha * m_fRangeY;

		m_tPos.x = m_tOrigin.x + dx + m_noiseDist(util::_rng);
		m_tPos.y = m_tOrigin.y + dy + m_noiseDist(util::_rng);

		m_fAlpha *= m_fDampingCoeff;
		m_fRangeX = -m_fRangeX;
		m_fRangeY = -m_fRangeY;
	}
}
