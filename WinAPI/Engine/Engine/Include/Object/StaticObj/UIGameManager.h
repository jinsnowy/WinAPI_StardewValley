#pragma once
#include "../../framework.h"
#include "UIPanel.h"
class UIGameManager
{
	DECLARE_SINGLE(UIGameManager)
private:
	class Clock {
	private:
		float m_fTimeCur = 0.f;
		static constexpr float m_fTimeUnit = 7.166f; // 게임 시간 10 분에 현실 시간 초 단위
	public:
		bool bAfterNoon = false;
		int hours = 6;
		int minutes = 0;
		int days = 1;
		int weekdays = 0;
	public:
		Clock() {};
		~Clock() {};
		void Tick(float dt);
		bool IsAfterNoon() { return bAfterNoon; }
		int GetWeekDays() const { return weekdays; }
		int GetDays() const { return days; }
		int GetHours()const { return hours; }
		int GetMinutes() const { return minutes; }
		void IncreaseTenMinutes() { Tick(m_fTimeUnit); }
	};
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
private:
	static constexpr float m_fUpperMargin = 20.f;
	static constexpr int m_iMoneyItemMargin = 6;
	static constexpr int m_iNumberWidth = 18;
	static constexpr int m_iMaxDigits = 8;
	static constexpr int m_iMoneyDrawStartX = 64;
	static constexpr int m_iMoneyDrawStartY = 192;
	static constexpr int m_iMaxHours = 26;
	static constexpr float interval = float(m_iMaxHours - 6) / 7.f;
	static constexpr int m_iUpperDisplayStartY = 20;
private:
	static constexpr float m_iItemListMargin = 8.f;
	static constexpr float m_fSmallNumberSize = 8.f;
	static constexpr int m_iItemListOffsetX = 20;
	static constexpr int m_iItemListOffsetY = 20;
private:
	bool m_bTickStart = false;
	bool m_bFastItemListSelect = true;
	class Player* m_pPlayer = nullptr;
	class UIPanel* m_pTimeUI = nullptr;
	class UIPanel* m_pFastItemListUI = nullptr;
	class UIPanel* m_pItemListUI = nullptr;

	vector<class Texture*> m_vecNeedleTex;
	vector<class Texture*> m_vecMoneyTex;
	vector<class Texture*> m_vecWeekDays;
	vector<class Texture*> m_vecNoon;
	vector<class Texture*> m_vecSmallNumbers;
	unique_ptr<Clock> m_clock = make_unique<Clock>();
	void DrawItemList(HDC hdc, float dt);
	void DrawCurrentTime(HDC hdc, float dt);
	void DrawCurrentMoney(HDC hdc, float dt);
public:
	void StartTick() { m_bTickStart = true; }
	void IncreaseTime() { m_clock->IncreaseTenMinutes(); }
	void SetPlayer(Player* pPlayer);
};

