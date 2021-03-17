#pragma once
#include "../../framework.h"
#include "UIGameTimer.h"
#include "UISeedStore.h"
#include "UIFastItemList.h"

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
	class Player* m_pPlayer = nullptr;
	unique_ptr<UIGameTimer> m_clockPanel = make_unique<UIGameTimer>();
	unique_ptr<UIFastItemList> m_itemPanel = make_unique<UIFastItemList>();
	unique_ptr<UISeedStore> m_storePanel = make_unique<UISeedStore>();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
private:
	bool m_bTickStart = false;
	bool m_bSeedStoreSelect = false;
	bool m_bFastItemListSelect = true;
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
	void StartTick() { m_bTickStart = true; }
	void GameTimerTick();
	void SetPlayer(Player* pPlayer);
	bool IsStoreSelect() const { return m_bSeedStoreSelect; }
	Player* AccessPlayer() { return m_pPlayer; }
	unsigned long long GetWorldTime() const;
};