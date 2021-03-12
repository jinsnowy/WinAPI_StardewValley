#pragma once
#include "../../framework.h"
#include "MovableObject.h"
#include "PlayerTool.h"

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
		TOOL_LEFT,
		TOOL_RIGHT,
		TOOL_DOWN,
		TOOL_UP,
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
	int m_iCurItemSel = 0;
	int m_iHP = 0;
	int m_iMoney = 5000;
public:
	const vector<class Item*>& AccessItemList() const { return m_vecItem; }
	int GetCurItemSel() const { return m_iCurItemSel; }
	int GetMoney() const { return m_iMoney; }
	void InitTexture();
	void InitAnimation();
	PlayerState GetState() const { return m_eState; }
	inline Pos GetCenterPos() const
	{
		return Pos(GetPos().x + GetSize().x / 2, GetPos().y);
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
	void BlockFoot(class Collider* pSrc, class Collider* pDst, float dt);
	void Hit(class Collider* pSrc, class Collider* pDst, float dt);
	void HitPixel(class Collider* pSrc, class Collider* pDst, float dt);
private:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	void KeyInput(float dt);
};

