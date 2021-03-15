#include "UIGameTimer.h"
#include "../../Application/Window.h"
#include "../../Resources/ResourceManager.h"
#include "../MoveObj/Player.h"

UIGameTimer::UIGameTimer()
{
	Init();
}

UIGameTimer::~UIGameTimer()
{
	SAFE_RELEASE(m_pPlayer);
	Safe_Release_VecList(m_vecMoneyTex);
	Safe_Release_VecList(m_vecNeedleTex);
	Safe_Release_VecList(m_vecWeekDays);
	Safe_Release_VecList(m_vecNoon);
}

bool UIGameTimer::Init()
{
    SetTexture("ClockBase", L"SV/Scene/ClockBase.bmp");
    SetColorKey(255, 255, 255);
    SetAsTextureSize();
    SetPos(GETRESOLUTION.x - GetSize().x, m_fUpperMargin);

    COLORREF chromaKey = RGB(255, 255, 255);
    m_vecNeedleTex = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/ClockNeedle/", chromaKey);
    m_vecWeekDays = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/WeekDays/", chromaKey);
    m_vecNoon = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/Noon/", chromaKey);
	m_vecMoneyTex = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/Money/", chromaKey);

	return true;
}

void UIGameTimer::Input(float dt)
{
    UI::Input(dt);
}

int UIGameTimer::Update(float dt)
{
    UI::Update(dt);

	m_clock->Tick(dt);

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

	Pos tPos = GetPos();

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
	vector<int> currentMoney;
	int money = m_pPlayer->GetMoney();
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

void UIGameTimer::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	if (m_pPlayer)
		m_pPlayer->AddRef();
}

void UIGameTimer::GameClock::Tick(float dt)
{
	m_fTimeCur += dt;
	if (m_fTimeCur >= m_fTimeUnit)
	{
		m_fTimeCur -= m_fTimeUnit;
		m_iMinutes += 10;
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
}
