#pragma once
#include "EffectInterface.h"

class Effect : public EffectInterface
{
protected:
	Pos m_tOrigin;
	class Object* m_pSubject = nullptr;
	float m_fDuration = 0.f;
	float m_fMaxDuration = 0.f;
private:
	bool IsOver() const{ return m_fDuration >= m_fMaxDuration; }
protected:
	explicit Effect(class Object* pObj, float duration);
	Effect(const Effect& effet) = delete;
	virtual ~Effect() = 0;
	virtual void Process(float dt) = 0;
	virtual bool Predicate() const { return false; }
public:
	virtual void Step(float dt) final;
};
