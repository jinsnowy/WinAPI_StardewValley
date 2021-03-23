#include "ChaseEffect.h"
#include "../Object/Object.h"
#include "../Math.h"

void ChaseEffect::Process(float dt)
{
    const Pos& tPos = m_pSubject->GetPos();
    const Pos& targetPos = m_pTarget->GetCenter();
    m_fDist = Math::Distance(targetPos, tPos);
    if (m_fDist <= m_fChaseRange)
    {
        Pos dir = (targetPos - tPos).GetNormalized();
        dir *= (m_fChaseSpeed * dt);
        m_pSubject->SetPos(tPos.x + dir.x, tPos.y + dir.y);
    }
}

bool ChaseEffect::Predicate() const
{
	return m_fDist <= 2.f;
}

ChaseEffect::ChaseEffect(Object* pSubject, Object* pTarget)
	:
    Effect(pSubject, FLT_MAX),
    m_pTarget(pTarget)
{
}

ChaseEffect::~ChaseEffect()
{
}
