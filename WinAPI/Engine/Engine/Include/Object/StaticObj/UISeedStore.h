#pragma once
#include "UI.h"

class UISeedStore : public UI
{
private:
	friend class GameManager;
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
	virtual UISeedStore* Clone() { throw EXCEPT(L"UISeedStore cloned"); return nullptr; }
private:
	bool m_bShow = false;
	class UIButton* m_pExitButton = nullptr;
	class UIScrollBar* m_pScrollBar = nullptr;
	vector<class Seed*> m_vecSellingSeeds;
	static constexpr float m_iMargin = 5.f;
	// 패널 한 칸에 대한 정보
	static constexpr float m_fPanelWidth = 938.f;
	static constexpr float m_fPanelHeight = 88.f;
	float m_fClickDelay = 0.f;
public:
	bool IsOn() const { return m_bShow; }
	void SetSeedStore(bool show);
private:
	void SetClickDelay() { m_fClickDelay = 0.5f; }
	void BuyingCallback(Collider* pSrc, Collider* pDst, float dt, int id);
	void SetUpItemBuyColliders();
	void Exit(float dt);
};