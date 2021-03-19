#include "Effect.h"

Effect::Effect(Object* pObj, float duration)
	: m_pSubject(pObj), m_fMaxDuration(duration)
{
	if (m_pSubject)
	{
		m_tOrigin = m_pSubject->GetPos();
	}
}

Effect::~Effect()
{
}

void Effect::Step(float dt)
{
	if (!m_bEnd)
	{
		m_fDuration += dt;
		Process(dt);
		m_bEnd = IsOver() || Predicate();
	}
}