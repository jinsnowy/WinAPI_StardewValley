#pragma once
#include "UI.h"
class UIFastItemList : public UI
{
private:
	friend class UIGameManager;
public:
	UIFastItemList();
	UIFastItemList(const UIFastItemList& ui) = delete;
	virtual ~UIFastItemList();
private:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual UI* Clone() { throw EXCEPT(L"UIFastItemList cloned"); return nullptr; }
private:
	class Player* m_pPlayer = nullptr;
	vector<class Texture*> m_vecSmallNumbers;
	static constexpr float m_iItemListMargin = 8.f;
	static constexpr float m_fSmallNumberSize = 8.f;
	static constexpr int m_iItemListOffsetX = 20;
	static constexpr int m_iItemListOffsetY = 20;
private:
	void SetPlayer(class Player* pPlayer);
};
