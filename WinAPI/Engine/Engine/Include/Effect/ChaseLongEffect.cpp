#include "ChaseLongEffect.h"
#include "../Object/Object.h"
#include "../Math.h"

void ChaseLongEffect::Process(float dt)
{
    const Pos& tPos = m_pSubject->GetPos();
    const Pos& targetPos = m_pTarget->GetCenter();
    m_fDist = Math::Distance(targetPos, tPos);
    if (m_fDist <= m_fChaseRange)
    {
        Pos dir = (targetPos - tPos).GetNormalized();

        float alpha = atan2(-dir.y, dir.x);
        float sin_val = m_fGain * W * cosf(W * m_fDuration) * dt;

        dir *= (m_fChaseSpeed * dt);

        float cosa = cosf(alpha);
        float sina = sinf(alpha);
        float px = tPos.x + dir.x + cosa * dt - sina * sin_val;
        float py = tPos.y + dir.y + sina * dt + cosa * sin_val;

        m_pSubject->SetPos(px, py);
    }
}

bool ChaseLongEffect::Predicate() const
{
    return m_fDist <= 2.f;
}

ChaseLongEffect::ChaseLongEffect(Object* pSubject, Object* pTarget)
    :
    Effect(pSubject, FLT_MAX),
    m_pTarget(pTarget)
{
}

ChaseLongEffect::~ChaseLongEffect()
{
}
