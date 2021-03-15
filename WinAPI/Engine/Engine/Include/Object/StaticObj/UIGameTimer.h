#pragma once
#include "UI.h"
class UIGameTimer : public UI
{
private:
	friend class UIGameManager;
	class GameClock {
	private:
		float m_fTimeCur = 0.f;
	public:
		bool m_bAfterNoon = false;
		int m_iHours = 6;
		int m_iMinutes = 0;
		int m_iDays = 1;
		int m_iWeekdays = 0;
	public:
		GameClock() {};
		GameClock(const GameClock&) = delete;
		~GameClock() {};
		void Tick(float dt);
		bool IsAfterNoon() { return m_bAfterNoon; }
		int GetWeekDays() const { return m_iWeekdays; }
		int GetDays() const { return m_iDays; }
		int GetHours()const { return m_iHours; }
		int GetMinutes() const { return m_iMinutes; }
	};
public:
	UIGameTimer();
	UIGameTimer(const UIGameTimer& ui) = delete;
	virtual ~UIGameTimer();
private:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual UI* Clone() { throw EXCEPT(L"UIGameTimer cloned"); return nullptr; }
private:
	static constexpr int m_iMoneyItemMargin = 6;
	static constexpr int m_iNumberWidth = 18;
	static constexpr int m_iMaxDigits = 8;
	static constexpr int m_iMoneyDrawStartX = 64;
	static constexpr int m_iMoneyDrawStartY = 192;
	static constexpr int m_iUpperDisplayStartY = 20;
	static constexpr float m_fUpperMargin = 20.f;

	static constexpr float m_fTimeUnit = 7.166f; // 게임 시간 10 분에 현실 시간 초 단위
	static constexpr int m_iMaxHours = 26;
	static constexpr float interval = float(m_iMaxHours - 6) / 7.f;
private:
	class Player* m_pPlayer = nullptr;
	unique_ptr<GameClock> m_clock = make_unique<GameClock>();
	vector<class Texture*> m_vecNeedleTex;
	vector<class Texture*> m_vecMoneyTex;
	vector<class Texture*> m_vecWeekDays;
	vector<class Texture*> m_vecNoon;
	void SetPlayer(class Player* pPlayer);
	void IncreaseTenMinutes() { m_clock->Tick(m_fTimeUnit); }
};