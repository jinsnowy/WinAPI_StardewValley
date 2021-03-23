#pragma once

#include "../framework.h"
class EffectInterface
{
public:
	EffectInterface();
	virtual ~EffectInterface() = 0;
protected:
	bool m_bEnd = false;
public:
	bool IsEnd() const { return m_bEnd; }
	virtual void Step(float dt) = 0;
};

typedef shared_ptr<EffectInterface> EffectPtr;
