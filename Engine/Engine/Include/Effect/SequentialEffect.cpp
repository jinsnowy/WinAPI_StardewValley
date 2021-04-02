#include "SequentialEffect.h"

SequentialEffect::~SequentialEffect()
{
}

void SequentialEffect::AddEffect(EffectPtr& next)
{
    if (m_CurEffect == nullptr)
    {
        m_CurEffect = std::move(next);
        return;
    }
    m_EffectQueue.push(std::move(next));
}

SequentialEffect::SequentialEffect(vector<EffectPtr> &effect_list)
{
    const auto iterEnd = effect_list.end();
    for (auto iter = effect_list.begin(); iter != iterEnd; ++iter)
    {
        AddEffect((*iter));
    }
}

void SequentialEffect::Step(float dt)
{
    m_CurEffect->Step(dt);

    if (m_CurEffect->IsEnd() && !IsEnd())
    {
        if (m_EffectQueue.size())
        {
            m_CurEffect = std::move(m_EffectQueue.front());
            m_EffectQueue.pop();
        }
        else 
        {
            m_bEnd = true;
        }
    }
}