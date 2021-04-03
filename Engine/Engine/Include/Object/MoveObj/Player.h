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
	bool m_bRing = false;
	static constexpr int m_iMaxItemNum = 36;
	static constexpr float m_fPlayerSpeed = 400.f;
	static constexpr float m_iMaxHP = 1000.f;
	static constexpr float m_iMaxMP = 1000.f;
	static constexpr float m_iUseMPUnit = 20.f;
	float m_iHP = m_iMaxHP;
	float m_iMP = m_iMaxMP;
	float m_fAttackRange = TILESIZE;
	int m_iCurItemSel = 0;
	int m_iMoney = 5000;
public:
	bool HasRing() const { return m_bRing; }
	float GetToolPower() const;
	const vector<class Item*>& AccessItemList() const { return m_vecItem; }
	int GetCurItemSel() const { return m_iCurItemSel; }
	void SetCurItemSel(int sel);
	Item* GetCurItem() const;
	int GetMoney() const { return m_iMoney; }
	bool Affordable(int cost) { return m_iMoney >= cost; }
	bool IsFull() const { return FindEmptyIndex() == -1; }
	bool SellItem(int index);
	bool SellItem(const string& itemTag);
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
	Item* GetItem(int index)
	{
		if (index < 0 || index >= m_iMaxItemNum) return nullptr;
		if (m_vecItem[index])
			m_vecItem[index]->AddRef();
		return m_vecItem[index];
	}
	void SwapItem(int index1, int index2)
	{
		if (index1 < 0 || index1 >= m_iMaxItemNum) return;
		if (index2 < 0 || index2 >= m_iMaxItemNum) return;
		swap(m_vecItem[index1], m_vecItem[index2]);
	}
	void Sleep()
	{
		m_iHP = m_iMaxHP;
		m_iMP = m_iMaxMP;
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
	void AddTool(Item* pItem);
	Item* FindItem(const string& itemTag);
	void AddItem(class Item* pItem);
	void DecreaseItem(class Item* pItem);
	int FindEmptyIndex() const;
private:
	bool HasTool(PlayerTool::ToolState tool) const
	{
		return m_vecItem[m_iCurItemSel] == (Item*)m_pPlayerTool->m_pTools[tool];
	}
	bool HasSwingTool() const
	{
		return HasTool(PlayerTool::TOOL_SICKLE) || HasTool(PlayerTool::TOOL_BLADE);
	}
	bool IsUsingTool() const { return m_eState == TOOL_USE; }
	bool IsToolSelected() const
	{
		if (!m_vecItem[m_iCurItemSel]) return false;
		return m_vecItem[m_iCurItemSel]->IsToolItem();
	}
	bool IsSeedSelected() const
	{
		if (!m_vecItem[m_iCurItemSel]) return false;
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

