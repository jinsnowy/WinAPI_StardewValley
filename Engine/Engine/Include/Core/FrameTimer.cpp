#include "FrameTimer.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"
using namespace chrono;

DEFINITION_SINGLE(FrameTimer)

FrameTimer::FrameTimer()
    : 
    m_hWnd(nullptr),
    m_Tic(steady_clock::now()),
    m_iFrame(0),
    m_TimeScale(1.f),
    m_fFPSTime(0.f),
    m_fFPS(0.f)
{
}

FrameTimer::~FrameTimer()
{
}

bool FrameTimer::Init(HWND hWnd)
{
    m_hWnd = hWnd;
    m_Tic = steady_clock::now();
    return true;
}

void FrameTimer::Reset()
{
    m_Tic = steady_clock::now();
    m_iFrame = 0;
    m_TimeScale = 1.f;
    m_fFPSTime = 0.f;
}

float FrameTimer::Peek()
{
    return duration_cast<duration<float>>(m_Tic - steady_clock::now()).count();
}

float FrameTimer::Tick()
{
    const auto tic = m_Tic;
    m_Tic = steady_clock::now();

    float dt = (duration_cast<duration<float>> (m_Tic - tic)).count();
    m_fFPSTime += dt;
    m_iFrame++;

    if (m_fFPSTime >= 1.f)
    {
        m_fFPS = m_iFrame / m_fFPSTime;
        m_fFPSTime = 0.f;
        m_iFrame = 0;

        wchar_t strFPS[64] = {};
        swprintf_s(strFPS, L"FPS : %.f\n", m_fFPS);
        SetWindowText(m_hWnd, strFPS);
        OutputDebugString(strFPS);
    }
    return dt * m_TimeScale;
}

float FrameTimer::ScaledTick()
{
   return Tick() * m_TimeScale;
}
