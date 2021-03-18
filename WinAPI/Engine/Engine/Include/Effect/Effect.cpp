#include "Effect.h"

Effect::Effect(const Pos& pos, float duration)
	: m_tPos(pos), m_fMaxDuration(duration)
{
}

Effect::~Effect()
{
}

Pos Effect::Next(float dt)
{
	if (!m_bEnd)
	{
		m_fDuration += dt;
		Process(dt);
		m_bEnd = IsOver() || Predicate();
	}
	return m_tPos;
}