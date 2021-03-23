#include "UIGameTimer.h"
#include "GameManager.h"
#include "UIPanel.h"
#include "../../Application/Window.h"
#include "../../Resources/ResourceManager.h"
#include "../MoveObj/Player.h"
#include "../../Effect/ShakeEffect.h"


UIGameTimer::UIGameTimer()
{
	Init();
}

UIGameTimer::~UIGameTimer()
{
	Safe_Release_VecList(m_vecMoneyTex);
	Safe_Release_VecList(m_vecNeedleTex);
	Safe_Release_VecList(m_vecWeekDays);
	Safe_Release_VecList(m_vecNoon);
	SAFE_RELEASE(m_pPlayerMPPanel);
}

bool UIGameTimer::Init()
{
    SetTexture("ClockBase", L"SV/Scene/UI/ClockBase.bmp");
    SetColorKey(255, 255, 255);
    SetAsTextureSize();
	SetNormalPos();

    COLORREF chromaKey = RGB(255, 255, 255);
    m_vecNeedleTex = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/ClockNeedle/", chromaKey);
    m_vecWeekDays = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/WeekDays/", chromaKey);
    m_vecNoon = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/Noon/", chromaKey);
	m_vecMoneyTex = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/Money/", chromaKey);

	m_pPlayerMPPanel = Object::CreateObject<UIPanel>("PlayerMPUI");
	m_pPlayerMPPanel->SetTexture("PlayerMPUI", L"SV/Scene/UI/Gauge.bmp");
	m_pPlayerMPPanel->SetColorKey(255, 255, 255);
	m_pPlayerMPPanel->SetAsTextureSize();
	Size tSize = m_pPlayerMPPanel->GetSize();
	m_pPlayerMPPanel->SetPos(float(GETRESOLUTION.x) - tSize.x - 10.f, float(GETRESOLUTION.y) - tSize.y - 10.f);
	return true;
}

void UIGameTimer::Input(float dt)
{
    UI::Input(dt);
	if (PLAYER->GetMPRemainRatio() < m_iMPShakeStart)
	{
		// 체력바 떨림
		if (!m_pPlayerMPPanel->HasEffect())
		{
			EffectPtr pEffect = make_shared<ShakeEffect>(m_pPlayerMPPanel, FLT_MAX, 0.01f, 3.f, 0.f, 8.0f, 1.0f);
			m_pPlayerMPPanel->SetEffect(pEffect);
		}
	}
	else {
		if (m_pPlayerMPPanel->HasEffect())
		{
			m_pPlayerMPPanel->SetEffect(nullptr);
		}
	}
}

int UIGameTimer::Update(float dt)
{
    UI::Update(dt);
	m_pPlayerMPPanel->Update(dt);
	bool ticked = m_clock->Tick(dt);
	GAME_MANAGER->Acknowleged(this);

    return 0;
}

int UIGameTimer::LateUpdate(float dt)
{
    UI::LateUpdate(dt);
    return 0;
}

void UIGameTimer::Collision(float dt)
{
    UI::Collision(dt);
}

void UIGameTimer::Draw(HDC hdc, float dt)
{
    UI::Draw(hdc, dt);

	m_pPlayerMPPanel->Draw(hdc, dt);
	// 체력 바
	Pos tPos = m_pPlayerMPPanel->GetPos();
	float ratio = PLAYER->GetMPRemainRatio();
	RECT rc = {};
	rc.left = tPos.x + m_iMPPanelStartX;
	rc.top = tPos.y + m_iMPPanelStartY + int((1 - ratio) * m_iMPPanelSizeY);
	rc.right = rc.left + m_iMPPanelSizeX;
	rc.bottom = rc.top + int(ratio * m_iMPPanelSizeY);

	COLORREF blend = RGB(max(int((1 - ratio) * 255.f), 10), max(int((ratio) * 255.f), 10), 0);
	DrawColorRectWithOutBorder(hdc, rc, blend);

	// 시계 바늘
	int hours = m_clock->GetHours();
	if (hours >= 0 && hours <= 5)
	{
		hours += 24;
	}
	int iCur;
	if (hours >= m_iMaxHours)
	{
		iCur = int(m_vecNeedleTex.size() - 1);
	}
	else {
		iCur = int((hours - 6) / interval);
	}

	m_vecNeedleTex[iCur]->DrawImageAt(hdc, GetPos());
	tPos = GetPos();

	// 월화수목금토일
	tPos.x += 140;
	tPos.y += m_iUpperDisplayStartY;
	m_vecWeekDays[m_clock->GetWeekDays()]->DrawImageAt(hdc, tPos);

	// 날짜
	int days = m_clock->GetDays();
	tPos.x += 50;
	tPos.y += 4;
	RESOURCE_MANAGER->DrawFontsAt(hdc, to_string(days), tPos.x, tPos.y, RIGHT, 2);

	// 오전 오후
	tPos = GetPos();
	tPos.x += 105;
	tPos.y += 114;
	m_vecNoon[m_clock->IsAfterNoon()]->DrawImageAt(hdc, tPos);

	// 시간
	tPos.x += 45;
	tPos.y += 4;
	ostringstream ss;
	ss << m_clock->GetHours() % 13 << ":" << setw(2) << setfill('0') << m_clock->GetMinutes();
	RESOURCE_MANAGER->DrawFontsAt(hdc, ss.str(), tPos.x, tPos.y, RIGHT, 5);

	// 돈
	float delta = dt;
	vector<int> currentMoney;
	int money = PLAYER->GetMoney();
	if (m_PlayerMoney != money)
	{
		while(delta >= m_fMoneyPrintPeriod && m_PlayerMoney != money)
		{
			delta -= m_fMoneyPrintPeriod;
			if (m_PlayerMoney > money)
			{
				--m_PlayerMoney;
			}
			else 
			{
				++m_PlayerMoney;
			}
		}
		money = m_PlayerMoney;
	}

	int digit = 0;
	do {
		digit = money % 10;
		money /= 10;
		currentMoney.push_back(digit);
	} while (money > 0);

	int stX = GetPos().x + m_iMoneyDrawStartX + (m_iMaxDigits - 1) * (m_iNumberWidth + m_iMoneyItemMargin);
	int stY = GetPos().y + m_iMoneyDrawStartY;

	int nDigits = (int)currentMoney.size();
	for (int i = 0; i < nDigits; ++i)
	{
		m_vecMoneyTex[currentMoney[i]]->DrawImageAt(hdc, Pos(stX, stY));
		stX -= (m_iNumberWidth + m_iMoneyItemMargin);
	}
}

void UIGameTimer::SetNormalPos()
{
	SetPos(GETRESOLUTION.x - GetSize().x, m_fUpperMargin);
}

bool UIGameTimer::GameClock::Tick(float dt)
{
	bool tick = false;
	m_fTimeCur += dt;
	if (m_fTimeCur >= m_fTimeUnit)
	{
		tick = true;
		m_fTimeCur -= m_fTimeUnit;
		m_iMinutes += 10;
		++m_iWorldTime;
		if (m_iMinutes == 60)
		{
			++m_iHours;
			m_iMinutes = 0;
			if (m_iHours == 24)
			{
				++m_iDays;
				m_iWeekdays = (m_iWeekdays + 1) % 7;
				m_iDays = m_iDays == 31 ? 1 : m_iDays;
			}
			m_iHours %= 24;
			m_bAfterNoon = m_iHours >= 12 ? true : false;
		}
	}
	return tick;
}

float UIGameTimer::GameClock::GetDayDarkness() const
{
	if (m_iHours >= 6 && m_iHours <= 18)
		return 0.f;
	else 
	{
		return min((abs(float(m_iHours) - 12.f) - 6.f) / 6.f, m_fMaxDarkness);
	}
}

unsigned long long UIGameTimer::GetWorldTime() const
{
	return m_clock->m_iWorldTime;
}
