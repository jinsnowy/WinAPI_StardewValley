#include "Monster.h"
#include "../../Scene/GameScene.h"
Monster::Monster()
{
	m_eObjType = OBJ_MONSTER;
}

Monster::Monster(const Monster& obj)
    : MovableObject(obj)
{
}

Monster::~Monster()
{
}

bool Monster::Init()
{
	Animation* pAnim = CreateAnimation("GhostAnimation");
	AddAnimationClip("GhostDown",
		            AT_ATLAS, AO_LOOP,
		            0.0f, WALKSPEED,
		            4, 1,
		            0, 0,
		            4, 1,
		            0.f, "GhostDown", L"SV/Object/Monster/Ghost/GhostDown.bmp");
	SetClipColorKey("GhostDown", 255, 255, 255);
	AddAnimationClip("GhostLeft",
		AT_ATLAS, AO_LOOP,
		0.0f, WALKSPEED,
		4, 1,
		0, 0,
		4, 1,
		0.f, "GhostLeft", L"SV/Object/Monster/Ghost/GhostLeft.bmp");
	SetClipColorKey("GhostLeft", 255, 255, 255);
	AddAnimationClip("GhostRight",
		AT_ATLAS, AO_LOOP,
		0.0f, WALKSPEED,
		4, 1,
		0, 0,
		4, 1,
		0.f, "GhostRight", L"SV/Object/Monster/Ghost/GhostRight.bmp");
	SetClipColorKey("GhostRight", 255, 255, 255);
	AddAnimationClip("GhostUp",
		AT_ATLAS, AO_LOOP,
		0.0f, WALKSPEED,
		4, 1,
		0, 0,
		4, 1,
		0.f, "GhostUp", L"SV/Object/Monster/Ghost/GhostUp.bmp");
	SetClipColorKey("GhostUp", 255, 255, 255);
    pAnim->SetDefaultClip("GhostDown");
    SAFE_RELEASE(pAnim);

	SetSpeed(m_fMonsterSpeed);
	SetTexture("Ghost", L"SV/Object/Monster/Ghost/GhostDown.bmp");
	SetSize(64.f, 96.f);
	StateTransit(3);
	DisableMovement();
    return true;
}

void Monster::Input(float dt)
{
	MovableObject::Input(dt);

	Pos prev = GetPos();
	if (m_bMoveEnabled)
	{
		switch (m_eState)
		{
		case MonsterState::MOVE_DOWN:
			MoveYFromSpeed(dt, MD_FRONT);
			break;
		case MonsterState::MOVE_UP:
			MoveYFromSpeed(dt, MD_BACK);
			break;
		case MonsterState::MOVE_LEFT:
			MoveXFromSpeed(dt, MD_BACK);
			break;
		case MonsterState::MOVE_RIGHT:
			MoveXFromSpeed(dt, MD_FRONT);
			break;
		}
	}

	if (m_bMove)
	{
		// 다음 타일이 갈 수 없다면,
		const GameScene* gameScene = static_cast<GameScene*>(m_pScene);
		if (gameScene->IsBlockTile(GetCenterPos()))
		{
			SetPos(prev);
			int i = 0;
			do
			{
				i = rand() % 4;
			} while (i == (int)m_eState);
			StateTransit(i);
		}
	}
}

void Monster::StateTransit(int iNext)
{
	m_eState = static_cast<MonsterState>(iNext);
	switch (m_eState)
	{
	case MonsterState::MOVE_DOWN:
		m_pAnimation->ChangeClip("GhostDown");
		break;
	case MonsterState::MOVE_UP:
		m_pAnimation->ChangeClip("GhostUp");
		break;
	case MonsterState::MOVE_LEFT:
		m_pAnimation->ChangeClip("GhostLeft");
		break;
	case MonsterState::MOVE_RIGHT:
		m_pAnimation->ChangeClip("GhostRight");
		break;
	}
}

int Monster::Update(float dt)
{
    MovableObject::Update(dt);
    return 0;
}

int Monster::LateUpdate(float dt)
{
    MovableObject::LateUpdate(dt);
    return 0;
}

void Monster::Collision(float dt)
{
    MovableObject::Collision(dt);
}

void Monster::Draw(HDC hDC, float dt)
{
    MovableObject::Draw(hDC, dt);
}

Monster* Monster::Clone()
{
    return new Monster(*this);
}

void Monster::Save(FILE* pFile)
{
    MovableObject::Save(pFile);
}

void Monster::Load(FILE* pFile)
{
    MovableObject::Load(pFile);
}

void Monster::LateInit()
{
	EnableMovement();
}
