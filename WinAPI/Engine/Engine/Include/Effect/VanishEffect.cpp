#include "VanishEffect.h"
#include "../Object/Object.h"

VanishEffect::VanishEffect(Object* pObj, float duration, const Pos& tMoveMent)
    :
    Effect(pObj, duration),
    m_fDeltaOfAlpha(255*(1.0/duration))
{
    m_tDeltaPos = tMoveMent * ( 1.0 / duration);
    m_pSubject->EnableTransparentEffect();
}

VanishEffect::~VanishEffect()
{
}

void VanishEffect::Process(float dt)
{
    m_iAlpha -= (unsigned char) (m_fDeltaOfAlpha * dt);
    m_pSubject->SetPos(m_pSubject->GetPos() + m_tDeltaPos * dt);
    m_pSubject->SetAlpha(m_iAlpha);
}

bool VanishEffect::Predicate() const
{
    return m_iAlpha < 10;
}
