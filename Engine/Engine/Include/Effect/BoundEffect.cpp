#include "BoundEffect.h"
#include "../Object/Object.h"

BoundEffect::BoundEffect(Object* pObj, float duration,
						int maxBounce,
						float fAngle, float fVelo,
						float YLimit)
	: Effect(pObj, duration)
{
	m_iMaxBounceNum = maxBounce;
	m_fYLimit = YLimit;
	m_fVeloX = fVelo * cosf(fAngle);
	m_fVeloY = fVelo * sinf(fAngle);
}

BoundEffect::~BoundEffect()
{
}

void BoundEffect::Process(float dt)
{
	m_fVeloY -= GRAVITY * dt;

	float dx = m_fVeloX * dt;
	float dy = m_fVeloY * dt;

	m_tOrigin.x += dx;
	m_tOrigin.y -= dy;
	m_pSubject->SetPos(m_tOrigin);

	if (m_tOrigin.y >= m_fYLimit && m_iBounceCount < m_iMaxBounceNum)
	{
		m_fVeloX *= m_fCoeff;
		m_fVeloY *= -m_fCoeff;
		++m_iBounceCount;
	}
}

bool BoundEffect::Predicate() const
{
	return (m_iBounceCount == m_iMaxBounceNum);
}
