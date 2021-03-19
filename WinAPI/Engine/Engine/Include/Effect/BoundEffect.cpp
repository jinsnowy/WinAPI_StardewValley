#include "BoundEffect.h"

BoundEffect::BoundEffect(const Pos& origin, float duration,
						int maxBounce,
						float fAngle, float fVelo,
						float YLimit)
	: Effect(origin, duration)
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

	m_tPos.x += dx;
	m_tPos.y -= dy;

	if (m_tPos.y >= m_fYLimit && m_iBounceCount < m_iMaxBounceNum)
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
