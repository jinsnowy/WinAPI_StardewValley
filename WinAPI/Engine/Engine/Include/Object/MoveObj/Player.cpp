#include <cassert>
#include "Player.h"
#include "../../Math.h"
#include "../../Scene/Scene.h"
#include "../../Core/Input.h"
#include "../../Resources/ResourceManager.h"
#include "../../Resources/Texture.h"
#include "../../Collider/ColliderRect.h"
#include "../../Collider/ColliderPixel.h"
#include "../../Collider/CollisionManager.h"
#include "../../Core/Camera.h"
#include "../../Animation/Animation.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/GameScene.h"
#include "../../Sound/SoundManager.h"
#include "PlayerTool.h"
#include "../Item/Item.h"
#include "../Item/Tool.h"

Player::Player()
	:
	m_tPrev(Pos())
{
}

Player::~Player()
{
	Safe_Release_VecList(m_vecItem);
}

void Player::InitTexture()
{
	Texture* pTex;
	pTex = RESOURCE_MANAGER->LoadTexture("IdleRight", L"Player/SV/IdleRight.bmp");
	pTex->SetColorKey(255, 255, 255);
	SAFE_RELEASE(pTex);
	pTex = RESOURCE_MANAGER->LoadTexture("IdleLeft", L"Player/SV/IdleLeft.bmp");
	pTex->SetColorKey(255, 255, 255);
	SAFE_RELEASE(pTex);
	pTex = RESOURCE_MANAGER->LoadTexture("IdleDown", L"Player/SV/IdleDown.bmp");
	pTex->SetColorKey(255, 255, 255);
	SAFE_RELEASE(pTex);
	pTex = RESOURCE_MANAGER->LoadTexture("IdleUp", L"Player/SV/IdleUp.bmp");
	pTex->SetColorKey(255, 255, 255);
	SAFE_RELEASE(pTex);
}

void Player::InitAnimation()
{
	Animation* pAnim = CreateAnimation("PlayerAnimation");
	SAFE_RELEASE(pAnim);
	wstring basePath;
	vector<wstring> vecFileName;
	wstringstream ss;

	// Walk 애니메이션
	{
		basePath = L"Player/SV/WalkRight/";
		vecFileName.clear();
		for (int i = 1; i <= 6; ++i)
		{
			ss << basePath << L"WalkRight" << i << ".bmp";
			vecFileName.push_back(ss.str());
			ss.clear();
			ss.str(L"");
		}
		AddAnimationClip("WalkRight",
			AT_FRAME, AO_LOOP,
			0.0f, 0.8f,
			6, 1,
			0, 0,
			6, 1,
			0.f, "WalkRight_Anim", vecFileName);
		SetClipColorKey("WalkRight", 255, 255, 255);

		basePath = L"Player/SV/WalkLeft/";
		vecFileName.clear();
		for (int i = 1; i <= 6; ++i)
		{
			ss << basePath << L"WalkLeft" << i << ".bmp";
			vecFileName.push_back(ss.str());
			ss.clear();
			ss.str(L"");
		}
		AddAnimationClip("WalkLeft",
			AT_FRAME, AO_LOOP,
			0.0f, 0.8f,
			6, 1,
			0, 0,
			6, 1,
			0.f, "WalkLeft_Anim", vecFileName);
		SetClipColorKey("WalkLeft", 255, 255, 255);

		basePath = L"Player/SV/WalkDown/";
		vecFileName.clear();
		for (int i = 1; i <= 9; ++i)
		{
			ss << basePath << L"WalkDown" << i << ".bmp";
			vecFileName.push_back(ss.str());
			ss.clear();
			ss.str(L"");
		}
		AddAnimationClip("WalkDown",
			AT_FRAME, AO_LOOP,
			0.0f, 0.8f,
			9, 1,
			0, 0,
			9, 1,
			0.f, "WalkDown_Anim", vecFileName);
		SetClipColorKey("WalkDown", 255, 255, 255);

		basePath = L"Player/SV/WalkUp/";
		vecFileName.clear();
		for (int i = 1; i <= 9; ++i)
		{
			ss << basePath << L"WalkUp" << i << ".bmp";
			vecFileName.push_back(ss.str());
			ss.clear();
			ss.str(L"");
		}
		AddAnimationClip("WalkUp",
			AT_FRAME, AO_LOOP,
			0.0f, 0.8f,
			9, 1,
			0, 0,
			9, 1,
			0.f, "WalkUp_Anim", vecFileName);
		SetClipColorKey("WalkUp", 255, 255, 255);
	}

	// Swing 애니메이션
	{
		basePath = L"Player/SV/SwingRight/";
		vecFileName.clear();
		for (int i = 1; i <= 5; ++i)
		{
			ss << basePath << L"SwingRight" << i << ".bmp";
			vecFileName.push_back(ss.str());
			ss.clear();
			ss.str(L"");
		}
		AddAnimationClip("SwingRight",
			AT_FRAME, AO_ONCE_RETURN,
			0.0f, SWINGSPEED,
			5, 1,
			0, 0,
			5, 1,
			0.f, "SwingRight_Anim", vecFileName);
		SetClipColorKey("SwingRight", 255, 255, 255);
		SetClipNextState("SwingRight", IDLE_RIGHT);

		basePath = L"Player/SV/SwingLeft/";
		vecFileName.clear();
		for (int i = 1; i <= 5; ++i)
		{
			ss << basePath << L"SwingLeft" << i << ".bmp";
			vecFileName.push_back(ss.str());
			ss.clear();
			ss.str(L"");
		}
		AddAnimationClip("SwingLeft",
			AT_FRAME, AO_ONCE_RETURN,
			0.0f, SWINGSPEED,
			5, 1,
			0, 0,
			5, 1,
			0.f, "SwingLeft_Anim", vecFileName);
		SetClipColorKey("SwingLeft", 255, 255, 255);
		SetClipNextState("SwingLeft", IDLE_LEFT);

		basePath = L"Player/SV/SwingUp/";
		vecFileName.clear();
		for (int i = 1; i <= 5; ++i)
		{
			ss << basePath << L"SwingUp" << i << ".bmp";
			vecFileName.push_back(ss.str());
			ss.clear();
			ss.str(L"");
		}
		AddAnimationClip("SwingUp",
			AT_FRAME, AO_ONCE_RETURN,
			0.0f, SWINGSPEED,
			5, 1,
			0, 0,
			5, 1,
			0.f, "SwingUp_Anim", vecFileName);
		SetClipColorKey("SwingUp", 255, 255, 255);
		SetClipNextState("SwingUp", IDLE_UP);

		basePath = L"Player/SV/SwingDown/";
		vecFileName.clear();
		for (int i = 1; i <= 5; ++i)
		{
			ss << basePath << L"SwingDown" << i << ".bmp";
			vecFileName.push_back(ss.str());
			ss.clear();
			ss.str(L"");
		}
		AddAnimationClip("SwingDown",
			AT_FRAME, AO_ONCE_RETURN,
			0.0f, SWINGSPEED,
			5, 1,
			0, 0,
			5, 1,
			0.f, "SwingDown_Anim", vecFileName);
		SetClipColorKey("SwingDown", 255, 255, 255);
		SetClipNextState("SwingDown", IDLE_DOWN);
	}

	// Tool 애니메이션
	{
		basePath = L"Player/SV/ToolDown/";
		vecFileName.clear();
		for (int i = 1; i <= 3; ++i)
		{
			ss << basePath << L"ToolDown" << i << ".bmp";
			vecFileName.push_back(ss.str());
			ss.clear();
			ss.str(L"");
		}
		AddAnimationClip("ToolDown",
			AT_FRAME, AO_ONCE_RETURN,
			0.0f, TOOLSPEED,
			3, 1,
			0, 0,
			3, 1,
			0.f, "ToolDown_Anim", vecFileName);
		SetClipColorKey("ToolDown", 255, 255, 255);
		SetClipNextState("ToolDown", IDLE_DOWN);

		basePath = L"Player/SV/ToolUp/";
		vecFileName.clear();
		for (int i = 1; i <= 3; ++i)
		{
			ss << basePath << L"ToolUp" << i << ".bmp";
			vecFileName.push_back(ss.str());
			ss.clear();
			ss.str(L"");
		}
		AddAnimationClip("ToolUp",
			AT_FRAME, AO_ONCE_RETURN,
			0.0f, TOOLSPEED,
			3, 1,
			0, 0,
			3, 1,
			0.f, "ToolUp_Anim", vecFileName);
		SetClipColorKey("ToolUp", 255, 255, 255);
		SetClipNextState("ToolUp", IDLE_UP);

		basePath = L"Player/SV/ToolLeft/";
		vecFileName.clear();
		for (int i = 1; i <= 3; ++i)
		{
			ss << basePath << L"ToolLeft" << i << ".bmp";
			vecFileName.push_back(ss.str());
			ss.clear();
			ss.str(L"");
		}
		AddAnimationClip("ToolLeft",
			AT_FRAME, AO_ONCE_RETURN,
			0.0f, TOOLSPEED,
			3, 1,
			0, 0,
			3, 1,
			0.f, "ToolLeft_Anim", vecFileName);
		SetClipColorKey("ToolLeft", 255, 255, 255);
		SetClipNextState("ToolLeft", IDLE_LEFT);

		basePath = L"Player/SV/ToolRight/";
		vecFileName.clear();
		for (int i = 1; i <= 3; ++i)
		{
			ss << basePath << L"ToolRight" << i << ".bmp";
			vecFileName.push_back(ss.str());
			ss.clear();
			ss.str(L"");
		}
		AddAnimationClip("ToolRight",
			AT_FRAME, AO_ONCE_RETURN,
			0.0f, TOOLSPEED,
			3, 1,
			0, 0,
			3, 1,
			0.f, "ToolRight_Anim", vecFileName);
		SetClipColorKey("ToolRight", 255, 255, 255);
		SetClipNextState("ToolRight", IDLE_RIGHT);
	}
	
}

float Player::GetToolPower() const
{
	if(IsToolSelected())
		return static_cast<Tool*>(m_vecItem[m_iCurItemSel])->GetPower();
	return 0.f;
}

Rect Player::BuildSwingAttack(int dx, int dy)
{
	Rect rect = {};
	if (dx > 0)
	{
		rect.left = 0;
		rect.top = -TILESIZE - m_fAttackRange;
		rect.right = rect.left + m_fAttackRange;
		rect.bottom = rect.top + 2 * m_fAttackRange;
	}
	else if (dx < 0)
	{
		rect.left = - m_fAttackRange;
		rect.top = -TILESIZE - m_fAttackRange;
		rect.right = rect.left + m_fAttackRange;
		rect.bottom = rect.top + 2 * m_fAttackRange;
	}
	else if (dy < 0)
	{
		rect.left = - m_fAttackRange;
		rect.top = -TILESIZE - m_fAttackRange;
		rect.right = rect.left + 2 * m_fAttackRange;
		rect.bottom = rect.top + TILESIZE + m_fAttackRange;
	}
	else if (dy >= 0)
	{
		rect.left = - m_fAttackRange;
		rect.top = -TILESIZE;
		rect.right = rect.left + 2 * m_fAttackRange;
		rect.bottom = rect.top  + TILESIZE + m_fAttackRange;
	}
	return rect;
}

void Player::StateTransit(int iNext)
{
	m_eState = static_cast<PlayerState>(iNext);
	switch (m_eState)
	{
	case IDLE_LEFT:
	case IDLE_RIGHT:
	case IDLE_UP:
	case IDLE_DOWN:
		m_bEnableAnimation = false;
		break;
	default:
		m_bEnableAnimation = true;
		break;
	}
	switch (m_eState)
	{
	case IDLE_LEFT:
		SetTexture("IdleLeft");
		break;
	case IDLE_RIGHT:
		SetTexture("IdleRight");
		break;
	case IDLE_UP:
		SetTexture("IdleUp");
		break;
	case IDLE_DOWN:
		SetTexture("IdleDown");
		break;
	case WALK_LEFT:
		m_pAnimation->ChangeClip("WalkLeft");
		break;
	case WALK_RIGHT:
		m_pAnimation->ChangeClip("WalkRight");
		break;
	case WALK_DOWN:
		m_pAnimation->ChangeClip("WalkDown");
		break;
	case WALK_UP:
		m_pAnimation->ChangeClip("WalkUp");
		break;
	}
}

bool Player::Init()
{
	SetPos(700.0f, 700.0f);
	SetSize(64.0f, 128.0f);
	SetPivot(0.0f, 1.0f);
	SetSpeed(m_fPlayerSpeed);

	InitTexture();
	InitAnimation();

	AdvertiseFrom(CO_PLAYER);

	StateTransit(IDLE_RIGHT);

	ColliderRect* pRC = AddCollider<ColliderRect>("PlayerBody");
	pRC->SetRect(10.f, -100.f, 50.f, -10.f);
	pRC->AddCollisionFunction(CS_ENTER, this, &Player::Hit);
	pRC->AddCollisionFunction(CS_STAY, this, &Player::Hit);

	SAFE_RELEASE(pRC);

	ColliderRect* pFoot = AddCollider <ColliderRect>("PlayerFoot");
	pFoot->SetRect(0.f, -60.f, 60.f, 0.f);
	pFoot->AddCollisionFunction(CS_ENTER, this, &Player::HitFoot);
	pFoot->AddCollisionFunction(CS_STAY, this, &Player::HitFoot);
	SAFE_RELEASE(pFoot);

	m_pPlayerTool->SetPlayer(this);

	SOUND_MANAGER->LoadSound("InHouse_Walking", false, SD_EFFECT, "InHouse_Walking.mp3");

	for (int i = 0; i < 10; ++i)
	{
		char name[10] = { 0 };
		sprintf_s(name, "Item%d", i);
		INPUT->AddKey(name, '0' + i);
	}

	INPUT->AddKey("Item-", char(0x2d));
	INPUT->AddKey("Item=", char(0x3d));

	return true;
}

void Player::Input(float dt)
{
	MovableObject::Input(dt);
	ChangePlayerTool(dt);

	if (m_eState == TOOL_USE)
	{
		return;
	}

	m_tPrev = GetPos();
	const GameScene* gameScene = static_cast<GameScene*>(m_pScene);

	if (KEYPRESS("MoveUp"))
	{
		MoveYFromSpeed(dt, MD_BACK);
		StateTransit(WALK_UP);
	}
	else if (KEYPRESS("MoveDown"))
	{
		MoveYFromSpeed(dt, MD_FRONT);
		StateTransit(WALK_DOWN);
	}
	else if (KEYPRESS("MoveLeft"))
	{
		MoveXFromSpeed(dt, MD_BACK);
		StateTransit(WALK_LEFT);
	}
	else if (KEYPRESS("MoveRight"))
	{
		MoveXFromSpeed(dt, MD_FRONT);
		StateTransit(WALK_RIGHT);
	}
	
	if (m_bMove)
	{
		// 다음 타일이 갈 수 없다면,
		if(gameScene->IsBlockTile(GetCenterPos()))
		{
			SetPos(m_tPrev);
		}
		if (gameScene->GetSceneType() == SC_INHOUSE && SOUND_MANAGER->IsEnd(SD_EFFECT))
		{
			SOUND_MANAGER->PlaySound("InHouse_Walking");
		}
	}
	else
	{
		// 움직이지 않았다면,
		switch (m_eState)
		{
		case WALK_RIGHT:
			StateTransit(IDLE_RIGHT);
			break;
		case WALK_LEFT:
			StateTransit(IDLE_LEFT);
			break;
		case WALK_UP:
			StateTransit(IDLE_UP);
			break;
		case WALK_DOWN:
			StateTransit(IDLE_DOWN);
			break;
		}
		
		if (KEYDOWN("MouseLButton"))
		{
			COLLISION_MANAGER->ClickPoint();
		}

		if (KEYDOWN("MouseLButton") && IsIdleState() && IsToolSelected())
		{
			Pos tMousePos = MOUSEWORLDPOS;
			Pos tCenterPos = GetCenterPos();
			Pos tPos = GetPos();
			INDEX index = gameScene->GetIndexDiff(tMousePos, tCenterPos);
			if (max(abs(index.x), abs(index.y)) <= 1)
			{
				bool bSwingTool = IsSwingTool();
				if (index.x > 0)
				{
					bSwingTool ? m_pAnimation->ChangeClip("SwingRight") :
								m_pAnimation->ChangeClip("ToolRight");
					StateTransit(IDLE_RIGHT);
				}
				else if (index.x < 0)
				{
					bSwingTool ? m_pAnimation->ChangeClip("SwingLeft") :
								m_pAnimation->ChangeClip("ToolLeft");
					StateTransit(IDLE_LEFT);
				}
				else if (index.y < 0)
				{
					bSwingTool ? m_pAnimation->ChangeClip("SwingUp") :
								m_pAnimation->ChangeClip("ToolUp");
					StateTransit(IDLE_UP);
				}
				else if (index.y >= 0)
				{
					bSwingTool ? m_pAnimation->ChangeClip("SwingDown") :
								m_pAnimation->ChangeClip("ToolDown");
					StateTransit(IDLE_DOWN);
				}
					
				m_pPlayerTool->Play();
				StateTransit(TOOL_USE);

				if (bSwingTool)
				{
					// 낫/검 체크
					Rect attackRange = BuildSwingAttack(index.x, index.y);
					TRIGGER_RECTEVENT(GetCenterPos(), attackRange, "SwingTool");
				}
				else {
					// 도끼/곡괭이/호미 체크
					if (HasTool(PlayerTool::TOOL_AXE))
					{
						TRIGGER_CLICKEVENT(tMousePos, "AxeTool");
					}
					else if (HasTool(PlayerTool::TOOL_PICK))
					{
						TRIGGER_CLICKEVENT(tMousePos, "PickTool");
					}
					else if (HasTool(PlayerTool::TOOL_HOE))
					{
						static_cast<GameScene*>(m_pScene)->DigTile(tMousePos);
					}
				}
			}
		}
	}
}

int Player::Update(float dt)
{
	MovableObject::Update(dt);

	m_pPlayerTool->Update(dt);
	return 0;
}

int Player::LateUpdate(float dt)
{
	MovableObject::LateUpdate(dt);

	m_pPlayerTool->LateUpdate(dt);
	return 0;
}

void Player::Collision(float dt)
{
	MovableObject::Collision(dt);
}

void Player::Draw(HDC hDC, float dt)
{
  	MovableObject::Draw(hDC, dt);

	m_pPlayerTool->Draw(hDC, dt);

#ifdef _DEBUG
	wchar_t playerPos[32] = {};
	swprintf_s(playerPos, L"Pos: %.1f, %.1f", GetPos().x, GetPos().y);
	Pos tPos = m_tPos - m_tSize * m_tPivot;
	tPos -= CAMERA->GetTopLeft();
	TextOut(hDC, tPos.x, tPos.y - 10, playerPos, lstrlen(playerPos));
#endif

	if (SHOWCHECK(SHOW_TILEOPTION))
	{
		Pos tilePos = static_cast<GameScene*>(m_pScene)->GetTilePos(GetCenterPos());
		tilePos -= CAMERA->GetTopLeft();
		DrawRectWithColor(hDC, MakeRect(int(tilePos.x), int(tilePos.y), TILESIZE, TILESIZE),
			RGB(0, 200, 0));
	}
}

void Player::HitFoot(Collider* pSrc, Collider* pDst, float dt)
{
	if (pDst->GetTag() == "TileBlock")
	{
		SetPos(m_tPrev);
	}
}

void Player::Hit(Collider* pSrc, Collider* pDst, float dt)
{
	if (pDst->GetTag() == "ItemBody")
	{
		AddItem(static_cast<Item*>(pDst->GetObj()));
	}
}

void Player::Save(FILE* pFile)
{
}

void Player::Load(FILE* pFile)
{
}

void Player::ChangePlayerTool(float dt)
{
	if (KEYDOWN("Item1"))
	{
		m_iCurItemSel = 0;
	}
	else if (KEYDOWN("Item2"))
	{
		m_iCurItemSel = 1;
	}
	else if (KEYDOWN("Item3"))
	{
		m_iCurItemSel = 2;
	}
	else if (KEYDOWN("Item4"))
	{
		m_iCurItemSel = 3;
	}
	else if (KEYDOWN("Item5"))
	{
		m_iCurItemSel = 4;
	}
	else if (KEYDOWN("Item6"))
	{
		m_iCurItemSel = 5;
	}
	else if (KEYDOWN("Item7"))
	{
		m_iCurItemSel = 6;
	}
	else if (KEYDOWN("Item8"))
	{
		m_iCurItemSel = 7;
	}
	else if (KEYDOWN("Item9"))
	{
		m_iCurItemSel = 8;
	}
	else if (KEYDOWN("Item0"))
	{
		m_iCurItemSel = 9;
	}
	else if (KEYDOWN("Item-"))
	{
		m_iCurItemSel = 10;
	}
	else if (KEYDOWN("Item="))
	{
		m_iCurItemSel = 11;
	}
	if (m_iCurItemSel < m_vecItem.size())
	{
		Tool* pTool = dynamic_cast<Tool*>(m_vecItem[m_iCurItemSel]);
		if (pTool)
		{
			m_pPlayerTool->SetTool(pTool);
		}
	}
}

Item* Player::FindItem(const string& itemTag)
{
	for (Item* pItem : m_vecItem)
	{
		if (pItem->GetTag() == itemTag)
		{
			return pItem;
		}
	}
	return nullptr;
}

void Player::AddItem(Item* pItem)
{
	Item* exist = FindItem(pItem->GetTag());
	if (!exist)
	{
		exist = pItem->Clone();
		exist->EraseAllColiders();
		m_vecItem.push_back(exist);
	}
	else 
	{
		exist->Increase();
	}

	SOUND_MANAGER->PlaySound("ItemGet");
	pItem->Die();
}
