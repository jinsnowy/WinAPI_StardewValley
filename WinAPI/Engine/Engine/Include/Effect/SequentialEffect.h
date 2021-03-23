#pragma once
#include "Effect.h"

class SequentialEffect : public EffectInterface
{
protected:
	EffectPtr m_CurEffect = nullptr;
	queue<EffectPtr> m_EffectQueue;
protected:
	SequentialEffect();
	SequentialEffect(const SequentialEffect& effect) = delete;
	virtual ~SequentialEffect();
public:
	void AddEffect(const EffectPtr& next);
	virtual void Step(float dt) final;
};