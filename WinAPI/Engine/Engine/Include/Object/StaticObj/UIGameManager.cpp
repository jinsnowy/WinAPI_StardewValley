#include "UIGameManager.h"
#include "../../Resources/ResourceManager.h"
#include "../../Resources/Texture.h"
#include "../MoveObj/Player.h"
#include "../../Application/Window.h"
#include "../Object.h"
#include "../Item/Item.h"
#include "../../Core/Input.h"

DEFINITION_SINGLE(UIGameManager);

bool UIGameManager::Init()
{
	m_pTimeUI = Object::CreateObject<UIPanel>("TimeUI");
	m_pTimeUI->SetTexture("ClockBase", L"SV/Scene/ClockBase.bmp");
	m_pTimeUI->SetColorKey(255, 255, 255);
	m_pTimeUI->SetAsTextureSize();
	m_pTimeUI->SetPos(GETRESOLUTION.x - m_pTimeUI->GetSize().x, m_fUpperMargin);

	COLORREF chromaKey = RGB(255, 255, 255);
	m_vecMoneyTex = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/Money/", chromaKey);
	m_vecNeedleTex = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/ClockNeedle/", chromaKey);
	m_vecWeekDays = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/WeekDays/", chromaKey);
	m_vecNoon = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/Noon/", chromaKey);

	m_pFastItemListUI = Object::CreateObject<UIPanel>("FastItemListUI");
	m_pFastItemListUI->SetTexture("FastItemList", L"SV/Scene/FastItemList.bmp");
	m_pFastItemListUI->SetColorKey(255, 255, 255);
	m_pFastItemListUI->SetAsTextureSize();
	m_pFastItemListUI->SetPos(float(GETRESOLUTION.x / 2) - m_pFastItemListUI->GetSize().x / 2.f,
								float(GETRESOLUTION.y) - m_pFastItemListUI->GetSize().y);

	return true;
}

void UIGameManager::Input(float dt)
{
}

int UIGameManager::Update(float dt)
{
	if (m_bTickStart)
	{
		m_clock->Tick(dt);
	}
    return 0;
}

int UIGameManager::LateUpdate(float dt)
{
    return 0;
}

void UIGameManager::Collision(float dt)
{
}

void UIGameManager::Draw(HDC hdc, float dt)
{
	DrawCurrentTime(hdc, dt);
	DrawCurrentMoney(hdc, dt);
	DrawItemList(hdc, dt);
}


void UIGameManager::DrawItemList(HDC hdc, float dt)
{
	if (m_bFastItemListSelect)
	{
		m_pFastItemListUI->Draw(hdc, dt);

		
		Pos tOffset = m_pFastItemListUI->GetPos();
		tOffset.x += m_iItemListOffsetX;
		tOffset.y += m_iItemListOffsetY;

		int sel = m_pPlayer->GetCurItemSel();
		DrawRedRect(hdc, MakeRect(tOffset.x +sel*(m_iItemListMargin + 56.f), tOffset.y, 56, 56));

		const auto& itemList = m_pPlayer->AccessItemList();
		int size = min(12, int(itemList.size()));
		for (int i = 0; i < size; ++i)
		{
			itemList[i]->DrawImageAt(hdc, tOffset, true);
			tOffset.x += m_iItemListMargin + 56.f;
		}

	}
	else
	{
		m_pItemListUI->Draw(hdc, dt);
	}
}

void UIGameManager::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	if (m_pPlayer)
		m_pPlayer->AddRef();
}
void UIGameManager::DrawCurrentMoney(HDC hdc, float dt)
{
	vector<int> currentMoney;
	int money = m_pPlayer->GetMoney();
	int digit = 0;
	do {
		digit = money % 10;
		money /= 10;
		currentMoney.push_back(digit);
	} while (money > 0);

	int stX = m_pTimeUI->GetPos().x + m_iMoneyDrawStartX + (m_iMaxDigits - 1) * (m_iNumberWidth + m_iMoneyItemMargin);
	int stY = m_pTimeUI->GetPos().y + m_iMoneyDrawStartY;

	int nDigits = (int)currentMoney.size();
	for (int i = 0; i < nDigits; ++i)
	{
		m_vecMoneyTex[currentMoney[i]]->DrawImageAt(hdc, Pos(stX, stY));
		stX -= (m_iNumberWidth + m_iMoneyItemMargin);
	}
}
void UIGameManager::DrawCurrentTime(HDC hdc, float dt)
{
	m_pTimeUI->Draw(hdc, dt);
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

	m_vecNeedleTex[iCur]->DrawImageAt(hdc, m_pTimeUI->GetPos());

	Pos tPos = m_pTimeUI->GetPos();

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
	tPos = m_pTimeUI->GetPos();
	tPos.x += 105;
	tPos.y += 114;
	m_vecNoon[m_clock->IsAfterNoon()]->DrawImageAt(hdc, tPos);

	// 시간
	tPos.x += 45;
	tPos.y += 4;
	ostringstream ss;
	ss << m_clock->GetHours() % 13 << ":" << setw(2) << setfill('0') << m_clock->GetMinutes();
	RESOURCE_MANAGER->DrawFontsAt(hdc, ss.str(), tPos.x, tPos.y, RIGHT, 5);
}

UIGameManager::UIGameManager()
{

}

UIGameManager::~UIGameManager()
{
	SAFE_RELEASE(m_pPlayer);
	SAFE_RELEASE(m_pTimeUI);
	SAFE_RELEASE(m_pFastItemListUI);
	Safe_Release_VecList(m_vecMoneyTex);
	Safe_Release_VecList(m_vecNeedleTex);
	Safe_Release_VecList(m_vecWeekDays);
	Safe_Release_VecList(m_vecNoon);
}

void UIGameManager::Clock::Tick(float dt)
{
	m_fTimeCur += dt;
	if (m_fTimeCur >= m_fTimeUnit)
	{
		m_fTimeCur -= m_fTimeUnit;
		minutes += 10;
		if (minutes == 60)
		{
			++hours;
			minutes = 0;
			if (hours == 24)
			{
				++days;
				weekdays = (weekdays + 1) % 7;
				days = days == 31 ? 1 : days;
			}
			hours %= 24;
			bAfterNoon = hours >= 12 ? true : false;
		}
	}
}
