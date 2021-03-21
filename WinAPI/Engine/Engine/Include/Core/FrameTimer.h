#pragma once
#include "../framework.h"

class FrameTimer
{
	DECLARE_SINGLE(FrameTimer)
public:
	bool Init(HWND hWnd);
	void SetTimeScale(float timeScale) { m_TimeScale = timeScale; }
	void IncreaseTimeScale(float ratio) { m_TimeScale *= ratio; }
	void Reset();
	float Peek();
	float Tick();
	float ScaledTick();
private:
	HWND m_hWnd;
	chrono::steady_clock::time_point m_Tic;
	int m_iFrame;
	float m_fFPSTime;
	float m_fFPS;
	float m_TimeScale;
};
