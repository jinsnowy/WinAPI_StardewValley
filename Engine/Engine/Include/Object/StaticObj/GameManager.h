#pragma once
#include "../../framework.h"

class GameManager
{
	DECLARE_SINGLE(GameManager)
	friend class Npc;
private:
	class WateredTile
	{
	private:
		static constexpr unsigned long long m_iDroughtDuration = 6 * 24 * 2;
	private:
		bool m_bDrought = false;
		unsigned long long m_iWaterTime = 0;
		int m_iIndex = -1;
	public:
		WateredTile(int index);
		~WateredTile();
	public:
		void Reset();
		bool IsDrought() const { return m_bDrought; }
		int GetIndex() const { return m_iIndex; }
		void Update();
	};
	list<WateredTile> m_WateredTiles;
	unordered_set<int> m_setWateredTileIndices;
	list<class Plant*> m_plantList;
private:
	enum class PANEL_TYPE
	{
		PLAYER_INFO = 0,
		GAME_TIMER,
		FAST_ITEMLIST,
		SEED_STORE,
		PANEL_END,
	};
	typedef shared_ptr<class UI> UIPtr;
	class Player* m_pPlayer = nullptr;
	vector<UIPtr> m_uiPanels;
	bitset<(int)PANEL_TYPE::PANEL_END> m_uiStates;
	unsigned long long m_iWorldTime = 0;
private:
	bool Activated(int id) { return m_uiStates[id]; }
	class UIGameTimer* GetTimer() const;
	class UISeedStore* GetStore() const;
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
private:
	bool m_bPlayerInfoSelect = false;
public:
	Player* AccessPlayer() { return m_pPlayer; }
	void SetPlayer(Player* pPlayer);
	void AddWateredTile(int index);
	vector<int> GetDroughtTiles();
	bool IsWateredTile(int index) const;

	unsigned long long GetWorldTime() const;

	void UpdateFarm();
	void AddPlantList(class Plant* pPlant);
	void Start();
	void Acknowleged(UI* changedUI);
	void GameTimerFastForward();

	bool IsStoreSelect() const { return m_uiStates[(int)PANEL_TYPE::SEED_STORE]; }
	float GetDayDarkNess() const;
	void SleepUntilMorning();
};