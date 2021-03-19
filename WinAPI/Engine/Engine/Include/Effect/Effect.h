#pragma once
#include "../framework.h"
#include "../Object/Object.h"
class Effect
{
protected:
	Pos m_tOrigin;
	class Object* m_pSubject = nullptr;
	bool m_bEnd = false;
	float m_fDuration = 0.f;
	float m_fMaxDuration = 0.f;
private:
	bool IsOver() const{ return m_fDuration >= m_fMaxDuration; }
protected:
	explicit Effect(class Object* pObj, float duration);
	Effect(const Effect& effet) = delete;
	virtual void Process(float dt) = 0;
	virtual bool Predicate() const { return false; }
public:
	virtual ~Effect() = 0;
	bool IsEnd() const { return m_bEnd; }
	void Step(float dt);
};