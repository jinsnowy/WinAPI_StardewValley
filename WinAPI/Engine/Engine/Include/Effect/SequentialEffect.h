#pragma once
#include "Effect.h"

class SequentialEffect : public EffectInterface
{
protected:
	EffectPtr m_CurEffect = nullptr;
	queue<EffectPtr> m_EffectQueue;
private:
	void AddEffect(const EffectPtr& next);
public:
	SequentialEffect(int num, const EffectPtr&...);
	SequentialEffect(const SequentialEffect& effect) = delete;
	virtual ~SequentialEffect();
public:
	virtual void Step(float dt) final;
};