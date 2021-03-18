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
	virtual UIPlayerInfo* Clone() { throw EXCEPT(L"UIFastItemList cloned"); return nullptr; }
private:
	class UIPanel* m_pPlayerMPPanel = nullptr;

	static constexpr int m_iMPPanelStartX = 13;
	static constexpr int m_iMPPanelStartY = 56;
	static constexpr float m_iMPPanelSizeX = 25.f;
	static constexpr float m_iMPPanelSizeY = 225.f;
	vector<class Texture*> m_vecSmallNumbers;
	static constexpr float m_iItemListMargin = 8.f;
	static constexpr float m_fSmallNumberSize = 12.f;
	static constexpr int m_iItemListOffsetX = 20;
	static constexpr int m_iItemListOffsetY = 20;
	int m_iItemListOffset = 0;
};
