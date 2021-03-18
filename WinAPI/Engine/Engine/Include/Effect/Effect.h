#pragma once
#include "../framework.h"
class Effect
{
protected:
	bool m_bEnd = false;
	Pos m_tPos = Pos();
	float m_fDuration = 0.f;
	float m_fMaxDuration = 0.f;
private:
	bool IsOver() const{ return m_fDuration >= m_fMaxDuration; }
protected:
	explicit Effect(const Pos& pos, float duration);
	Effect(const Effect& effet) = delete;
	virtual void Process(float dt) = 0;
	virtual bool Predicate() const { return false; }
public:
	virtual ~Effect() = 0;
	bool IsEnd() const { return m_bEnd; }
	Pos Next(float dt);
};