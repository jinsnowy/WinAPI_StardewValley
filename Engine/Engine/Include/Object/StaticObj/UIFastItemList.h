#pragma once
#include "UI.h"
class UIFastItemList : public UI
{
private:
	friend class GameManager;
public:
	UIFastItemList();
	UIFastItemList(const UIFastItemList& ui) = delete;
	virtual ~UIFastItemList();
	void SellItem();
private:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual UIFastItemList* Clone() { throw EXCEPT(L"UIFastItemList cloned"); return nullptr; }
private:
	bool m_bDrag = false;
	int m_iClickItemIndex = -1;
	class Item* m_pClickItem = nullptr;
	class UIPanel* m_pItemInfo = nullptr;
	vector<class Texture*> m_vecSmallNumbers;
	static constexpr float m_iItemListMargin = 8.f;
	static constexpr float m_fSmallNumberSize = 12.f;
	static constexpr int m_iItemBlockSize = 56;
	static constexpr int m_iItemListOffsetX = 20;
	static constexpr int m_iItemListOffsetY = 20;
	int m_iItemListOffset = 0;
private:
	int GetClickIndex(const Pos& screenPos);
};
