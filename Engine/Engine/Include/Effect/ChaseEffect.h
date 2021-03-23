#pragma once
#include "Effect.h"
class ChaseEffect : public Effect
{
private:
	class Object* m_pTarget;
	static constexpr float m_fChaseRange = 100.f;
	static constexpr float m_fChaseSpeed = 350.f;
	float m_fDist = FLT_MAX;
protected:
	virtual void Process(float dt);
	virtual bool Predicate() const;
public:
	explicit ChaseEffect(Object* pSubject, Object* pTarget);
	~ChaseEffect();
};

