#pragma once
#include "UI.h"
class UIPlayerInfo : public UI
{
private:
	friend class GameManager;
public:
	UIPlayerInfo();
	UIPlayerInfo(const UIPlayerInfo& ui) = delete;
	virtual ~UIPlayerInfo();
private:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual UIPlayerInfo* Clone() { throw EXCEPT(L"UIPlayerInfo cloned"); return nullptr; }
private:
	bool m_bDrag = false;
	int m_iClickItemIndex = -1;
	class Item* m_pClickItem = nullptr;
	vector<class Texture*> m_vecSmallNumbers;
	static constexpr float m_iItemBlockSize = 56.f;
	static constexpr float m_iItemListMargin = 8.f;
	static constexpr float m_fSmallNumberSize = 12.f;
	static constexpr int m_iItemListOffsetX = 43;
	static constexpr int m_iItemListOffsetY = 43;
	int GetClickIndex(const Pos& screenPos);
};