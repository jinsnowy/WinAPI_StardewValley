#pragma once
#include "../../framework.h"

class GameManager
{
	DECLARE_SINGLE(GameManager)
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
		void Reset();
		bool IsDrought() const { return m_bDrought; }
		int GetIndex() const { return m_iIndex; }
		WateredTile(int index);
		void Update();
	};
	deque<WateredTile> m_queWateredTile;
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
	typedef unique_ptr<class UI> UIPtr;
	class Player* m_pPlayer = nullptr;
	vector<UIPtr> m_uiPanels;
	bitset<(int)PANEL_TYPE::PANEL_END> m_uiStates;
	bool Activated(int id) { return m_uiStates[id]; }
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
	void AddWateredTile(int index);
	vector<int> GetDroughtTiles();
	void UpdateFarm();
	bool IsWateredTile(int index)
	{
		return m_setWateredTileIndices.find(index) != m_setWateredTileIndices.end();
	}
	void AddPlantList(class Plant* pPlant);
	void SetSeedStore(bool bSelect);
	void Start();
	void GameTimerTick();
	void SetPlayer(Player* pPlayer);
	bool IsStoreSelect() const { return m_uiStates[(int)PANEL_TYPE::SEED_STORE]; }
	Player* AccessPlayer() { return m_pPlayer; }
	unsigned long long GetWorldTime() const;
	float GetDayDarkNess() const;
	void SleepUntilMorning();
};