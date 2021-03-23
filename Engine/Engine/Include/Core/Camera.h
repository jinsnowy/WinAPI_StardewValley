#pragma once
#include "../framework.h"

class Camera
{
	DECLARE_SINGLE(Camera)
private:
	Pos			m_tPos;
	Pos			m_tPivot;
	RESOLUTION	m_tClientRS;
	RESOLUTION	m_tWorldRS;
	class Object* m_pTarget;

public:
	bool Init(const Pos& tPos, const RESOLUTION& tRS, const RESOLUTION& tWorldRS);
	void Input(float dt);
	void Update(float dt);
	void Scroll(float x, float y);
	
	Pos GetPos() const { return m_tPos; }
	Pos GetPivot() const { return m_tPivot; }
	Pos GetTopLeft() const { return m_tPos - m_tPivot * m_tClientRS; }
	RESOLUTION GetClientRS() const { return m_tClientRS; }
	RESOLUTION GetWorldRS() const { return m_tWorldRS; }

	void ReleaseTarget();
	void SetTarget(class Object* pTarget);
	void SetPos(const Pos& pos)   { m_tPos = pos; }
	void SetPos(float x, float y) { m_tPos = Pos(x, y); }
	void SetPivot(const Pos& pivot) { m_tPivot = pivot; }
	void SetPivot(float x, float y) { m_tPivot.x = x; m_tPivot.y = y; }
	void SetClientResolution(const RESOLUTION& tRS)		{ m_tClientRS = tRS; }
	void SetWorldResolution(const RESOLUTION& tWorldRS) { m_tWorldRS = tWorldRS; }
	void SetWorldResolution(int x, int y) { m_tWorldRS = { x,y }; }
};

