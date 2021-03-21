#include "Effect.h"

namespace std
{
	template<>
	struct default_delete<class Effect>
	{
		void operator()(Effect* ptr)
		{
			delete ptr;
		}
	};
}
Effect::Effect(Object* pObj, float duration)
	: m_tOrigin(pObj->GetPos()), m_pSubject(pObj), m_fMaxDuration(duration)
{
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