#pragma once
#include "Effect.h"

class SequentialEffect : public EffectInterface
{
protected:
	EffectPtr m_CurEffect = nullptr;
	queue<EffectPtr> m_EffectQueue;
private:
	void AddEffect(EffectPtr& next);
public:
	SequentialEffect(vector<EffectPtr> &effect_list);
	virtual ~SequentialEffect();
public:
	virtual void Step(float dt) final;
};