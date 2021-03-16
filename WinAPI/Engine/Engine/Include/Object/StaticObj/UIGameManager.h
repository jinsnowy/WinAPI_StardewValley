#pragma once
#include "../../framework.h"
#include "UIGameTimer.h"
#include "UISeedStore.h"
#include "UIFastItemList.h"

class UIGameManager
{
	DECLARE_SINGLE(UIGameManager)
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
	void SetSeedStore(bool bSelect);
	void StartTick() { m_bTickStart = true; }
	void IncreaseTime() { m_clockPanel->IncreaseTenMinutes(); }
	void SetPlayer(Player* pPlayer);
};

