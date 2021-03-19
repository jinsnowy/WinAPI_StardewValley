#pragma once
#include "Effect.h"
class VanishEffect : public Effect
{
private:
	unsigned char m_iAlpha = 255;
	const unsigned char m_fDeltaOfAlpha;
	Pos m_tDeltaPos;
protected:
	virtual void Process(float dt);
	virtual bool Predicate() const;
public:
	explicit VanishEffect(Object* pObj, float duration, const Pos& to);
	~VanishEffect();
};