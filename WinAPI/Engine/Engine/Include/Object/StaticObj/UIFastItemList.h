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
private:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual UIFastItemList* Clone() { throw EXCEPT(L"UIFastItemList cloned"); return nullptr; }
private:
	vector<class Texture*> m_vecSmallNumbers;
	static constexpr float m_iItemListMargin = 8.f;
	static constexpr float m_fSmallNumberSize = 12.f;
	static constexpr int m_iItemListOffsetX = 20;
	static constexpr int m_iItemListOffsetY = 20;
	int m_iItemListOffset = 0;
};
