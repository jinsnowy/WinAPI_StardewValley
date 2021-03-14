#include "BoundEffect.h"

BoundEffect::BoundEffect()
{
}

BoundEffect::~BoundEffect()
{
}

void BoundEffect::Init(const Pos& origin, int maxBounce, float fAngle, float fVelo, float YLimit, float duration)
{
	m_tPos = origin;
	m_iMaxBounceNum = maxBounce;
	m_fYLimit = YLimit;
	m_fDuration = duration;
	m_fVeloX = fVelo * cosf(fAngle);
	m_fVeloY = fVelo * sinf(fAngle);
}

Pos BoundEffect::Next(float dt)
{
	if (m_fDeltaTime <= m_fDuration)
	{
		m_fDeltaTime += dt;

		m_fVeloY -= GRAVITY * dt;

		float dx = m_fVeloX * dt;
		float dy = m_fVeloY * dt;

		m_tPos.x += dx;
		m_tPos.y -= dy;

		if (m_tPos.y >= m_fYLimit && m_iBounceCount < m_iMaxBounceNum)
		{
			m_fVeloY = -m_fCoeff * m_fVeloY;
			++m_iBounceCount;
		}

		if (m_fDeltaTime > m_fDuration || m_iBounceCount == m_iMaxBounceNum)
		{
			m_bEnd = true;
		}
	}

	return m_tPos;
}
