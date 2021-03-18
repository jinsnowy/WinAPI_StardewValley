#pragma once
#include "../../framework.h"
#include "MovableObject.h"
#include "PlayerTool.h"
#include "../Item/Tool.h"

class Player : public MovableObject
{
	friend class PlayerTool;
	friend class GameScene;
	friend class SceneManager;
public:
	enum PlayerState
	{
		IDLE_LEFT,
		IDLE_RIGHT,
		IDLE_DOWN,
		IDLE_UP,
		WALK_LEFT,
		WALK_RIGHT,
		WALK_DOWN,
		WALK_UP,
		TOOL_USE,
	};
private:
	Pos m_tPrev;
	unique_ptr<PlayerTool> m_pPlayerTool = make_unique<PlayerTool>();
	PlayerState m_eState = PlayerState::IDLE_RIGHT;
	vector<class Item*> m_vecItem;
private:
	Player();
	Player(const Player& obj) = delete;
	~Player();
private:
	bool m_bMoveEnabled = true;
	static constexpr int m_iMaxItemNum = 36;
	static constexpr float m_fPlayerSpeed = 800.f;
	static constexpr float m_iMaxMP = 1000.f;
	static constexpr float m_iUseMPUnit = 20.f;
	float m_iHP = 1000.f;
	float m_iMP = m_iMaxMP;
	float m_fAttackRange = TILESIZE;
	int m_iCurItemSel = 0;
	int m_iMoney = 5000;
public:
	void DisableMovement() { m_bMoveEnabled = false; }
	void EnableMovement() { m_bMoveEnabled = true; }
	float GetToolPower() const;
	const vector<class Item*>& AccessItemList() const { return m_vecItem; }
	int GetCurItemSel() const { return m_iCurItemSel; }
	void SetCurItemSel(int sel) { m_iCurItemSel = sel; }
	Item* GetCurItem() const;
	int GetMoney() const { return m_iMoney; }
	bool Affordable(int cost) { return m_iMoney >= cost; }
	bool IsFull() const { return m_vecItem.size() == m_iMaxItemNum; }
	void BuyItem(class Item* pItem);
	PlayerState GetState() const { return m_eState; }
	inline Pos GetCenterPos() const
	{
		return Pos(GetPos().x + GetSize().x / 2, GetPos().y);
	}
	inline Rect GetBodyRect() const
	{
		return Rect(GetLeft(), GetTop(), GetRight(), GetBottom());
	}
	float GetMPRemainRatio() const
	{
		return m_iMP / m_iMaxMP;
	}
public:
	virtual void StateTransit(int iNext);
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual Player* Clone() { throw EXCEPT(L"Player cloned"); return nullptr; }
public:
	void HitFoot(class Collider* pSrc, class Collider* pDst, float dt);
	void Hit(class Collider* pSrc, class Collider* pDst, float dt);
private:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
private:
	void InitTexture();
	void InitAnimation();
	void ChangePlayerTool(float dt);
	Item* FindItem(const string& itemTag);
	void AddItem(class Item* pItem);
	void EraseItem(class Item* pItem);
private:
	bool HasTool(PlayerTool::ToolState tool) const
	{
		if (m_iCurItemSel >= m_vecItem.size()) return false;
		return m_vecItem[m_iCurItemSel] == (Item*)m_pPlayerTool->m_pTools[tool];
	}
	bool HasSwingTool() const
	{
		return HasTool(PlayerTool::TOOL_SICKLE) || HasTool(PlayerTool::TOOL_BLADE);
	}
	bool IsUsingTool() const { return m_eState == TOOL_USE; }
	bool IsToolSelected() const
	{
		if (m_iCurItemSel >= m_vecItem.size()) return false;
		 return m_vecItem[m_iCurItemSel]->IsToolItem();
	}
	bool IsSeedSelected() const
	{
		if (m_iCurItemSel >= m_vecItem.size()) return false;
		return m_vecItem[m_iCurItemSel]->IsSeedItem();
	}
	bool IsIdleState() const
	{
		return (m_eState == IDLE_DOWN) || (m_eState == IDLE_UP)
			|| (m_eState == IDLE_LEFT) || (m_eState == IDLE_RIGHT);
	}
	Rect BuildSwingAttack(int dx, int dy);
	void PlayToolAnimation(const INDEX& index);
	void MovePlayer(float dt);
	void AfterMove();
	void AfterStop();
	void ClickEventHandling();
	bool HasEnoughMP() const;
	void UseMP();
};

