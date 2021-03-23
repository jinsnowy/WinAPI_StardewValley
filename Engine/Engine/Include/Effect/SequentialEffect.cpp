#include "SequentialEffect.h"

SequentialEffect::SequentialEffect(int num, const EffectPtr&...)
{
    va_list ap;
    va_start(ap, num);

    for (int i = 0; i < num; i++)
    {
        AddEffect(va_arg(ap, EffectPtr));
    }

    va_end(ap);
}

SequentialEffect::~SequentialEffect()
{
}

void SequentialEffect::AddEffect(const EffectPtr& next)
{
    if (m_CurEffect == nullptr)
    {
        m_CurEffect = next;
        return;
    }
    m_EffectQueue.push(next);
}

void SequentialEffect::Step(float dt)
{
    m_CurEffect->Step(dt);

    if (m_CurEffect->IsEnd() && !IsEnd())
    {
        if (m_EffectQueue.size())
        {
            m_CurEffect = m_EffectQueue.front();
            m_EffectQueue.pop();
        }
        else 
        {
            m_bEnd = true;
        }
    }
}