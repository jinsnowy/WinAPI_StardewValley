#pragma once
#include "UI.h"

class UISeedStore : public UI
{
private:
	friend class UIGameManager;
public:
	UISeedStore();
	UISeedStore(const UISeedStore& ui) = delete;
	virtual ~UISeedStore();
private:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual UI* Clone() { throw EXCEPT(L"UISeedStore cloned"); return nullptr; }
private:
	int m_iCurSel = 0;
	class Player* m_pPlayer = nullptr;
	class UIButton* m_pExitButton = nullptr;
	vector<class Item*> m_vecSellingSeeds;
private:
	void SetPlayer(class Player* pPlayer);
	void Exit(float dt);
};