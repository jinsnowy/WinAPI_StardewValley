#include "UIButton.h"
#include "../../Core/Input.h"
#include "../../Collider/ColliderRect.h"
#include "../../Sound/SoundManager.h"

UIButton::UIButton()
	: m_bEnableCallback(false),
	m_bUseSound(false),
	m_eState(BUTTON_STATE::BS_NONE),
	m_tMouseOnImageOffset(),
	m_tMouseOutImageOffset()
{

}

UIButton::UIButton(const UIButton& ui)
	: UI(ui)
{
	m_tMouseOnImageOffset = ui.m_tMouseOnImageOffset;
	m_tMouseOutImageOffset = ui.m_tMouseOutImageOffset;
	m_bEnableCallback = ui.m_bEnableCallback;
	m_eState = BUTTON_STATE::BS_NONE;
}

UIButton::~UIButton()
{
}

bool UIButton::Init()
{
	ColliderRect* pColl = AddCollider<ColliderRect>("ButtonBody");

	pColl->AddCollisionFunction(CS_ENTER, this, &UIButton::MouseOn);
	pColl->AddCollisionFunction(CS_LEAVE, this, &UIButton::MouseOut);
	pColl->EnableUICollider(true);

	SAFE_RELEASE(pColl);
	return true;
}

void UIButton::Input(float dt)
{
	UI::Input(dt);
}

int UIButton::Update(float dt)
{
	UI::Update(dt);
	return 0;
}

int UIButton::LateUpdate(float dt)
{
	UI::LateUpdate(dt);
	
	if (m_eState != BUTTON_STATE::BS_NONE)
	{
		if (KEYPRESS("MouseLButton"))
		{
			m_eState = BUTTON_STATE::BS_CLICK;
		}
	}

	if (m_eState == BUTTON_STATE::BS_CLICK && KEYUP("MouseLButton"))
	{
		if (m_bEnableCallback)
		{
			m_BtnCallback(dt);
		}
	}

	return 0;
}

void UIButton::Collision(float dt)
{
	UI::Collision(dt);
}

void UIButton::Draw(HDC hdc, float dt)
{
	UI::Draw(hdc, dt);
}

UIButton* UIButton::Clone()
{
	return new UIButton(*this);
}

void UIButton::MouseOn(Collider* pSrc, Collider* pDst, float dt)
{
	if (pDst->GetTag() == "Mouse")
	{
		m_eState = BUTTON_STATE::BS_MOUSEON;
		if(m_bUseMouseOnOutImage)
			SetImageOffset(m_tMouseOnImageOffset);
		if (m_bUseSound)
		{
			SOUND_MANAGER->PlaySound(m_strSoundTag);
		}
	}
}

void UIButton::MouseOut(Collider* pSrc, Collider* pDst, float dt)
{
	if (pDst->GetTag() == "Mouse")
	{
		m_eState = BUTTON_STATE::BS_NONE;
		if (m_bUseMouseOnOutImage)
			SetImageOffset(m_tMouseOutImageOffset);
	}
}
