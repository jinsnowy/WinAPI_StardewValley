#include "ShakeEffect.h"

normal_distribution<float> ShakeEffect::m_noiseDist(0.f, 0.5f);

ShakeEffect::ShakeEffect(Object* pObj, float duration,
	float fPeriod, float fRangeX, float fRangeY, float fNoiseCoeff, float fDampingCoeff)
	: Effect(pObj, duration)
{
	m_fRangeX = max(fRangeX, 0.0f);
	m_fRangeY = max(fRangeY, 0.0f);

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

		float posX = m_tOrigin.x + m_fAlpha * m_fRangeX + m_noiseDist(util::_rng);
		float posY = m_tOrigin.y + m_fAlpha * m_fRangeY + m_noiseDist(util::_rng);

		m_pSubject->SetPos(posX, posY);

		m_fAlpha *= m_fDampingCoeff;
		m_fRangeX = -m_fRangeX;
		m_fRangeY = -m_fRangeY;
	}
}
