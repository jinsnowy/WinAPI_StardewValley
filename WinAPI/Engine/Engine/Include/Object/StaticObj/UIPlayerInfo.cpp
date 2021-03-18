#include "UIPlayerInfo.h"
#include "UIPanel.h";
#include "GameManager.h"
#include "../../Application/Window.h"
#include "../MoveObj/Player.h"
#include "../../Core/Input.h"
#include "../../Resources/ResourceManager.h"
#include "../../Effect/ShakeEffect.h"

UIPlayerInfo::UIPlayerInfo()
{
	Init();
}

UIPlayerInfo::~UIPlayerInfo()
{
	Safe_Release_VecList(m_vecSmallNumbers);
	SAFE_RELEASE(m_pPlayerMPPanel);
}

bool UIPlayerInfo::Init()
{
    SetTexture("FastItemList", L"SV/Scene/FastItemList.bmp");
    SetColorKey(255, 255, 255);
    SetAsTextureSize();
    SetPos(float(GETRESOLUTION.x / 2) - GetSize().x / 2.f,
        float(GETRESOLUTION.y) - GetSize().y);

	COLORREF chromaKey = RGB(255, 255, 255);
	m_vecSmallNumbers = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/SmallNumbers/", chromaKey);
	INPUT->AddKey("NextItemList", VK_TAB);

	m_pPlayerMPPanel = Object::CreateObject<UIPanel>("PlayerMPUI");
	m_pPlayerMPPanel->SetTexture("PlayerMPUI", L"SV/Scene/Gauge.bmp");
	m_pPlayerMPPanel->SetColorKey(255,255,255);
	m_pPlayerMPPanel->SetAsTextureSize();
	Size tSize = m_pPlayerMPPanel->GetSize();
	m_pPlayerMPPanel->SetPos(float(GETRESOLUTION.x) - tSize.x - 10.f, float(GETRESOLUTION.y) - tSize.y - 10.f);
	return true;
}

void UIPlayerInfo::Input(float dt)
{
    UI::Input(dt);
	if (KEYDOWN("NextItemList"))
	{
		m_iItemListOffset = (m_iItemListOffset + 12) % 36;
		PLAYER->SetCurItemSel((PLAYER->GetCurItemSel() + 12) % 36);
	}
	if (PLAYER->GetMPRemainRatio() < m_iMPShakeStart)
	{
		// 체력바 떨림
		if (!m_pPlayerMPPanel->AccessEffect())
		{
			m_pPlayerMPPanel->SetEffect(new ShakeEffect(m_pPlayerMPPanel->GetPos(), FLT_MAX, 5.f, 5.f, 10.f, 1.0f));
		}
	}
	else {
		if(m_pPlayerMPPanel->AccessEffect())
		{
			m_pPlayerMPPanel->SetEffect(nullptr);
		}
	}
}

int UIPlayerInfo::Update(float dt)
{
    UI::Update(dt);
	m_pPlayerMPPanel->Update(dt);
    return 0;
}

int UIPlayerInfo::LateUpdate(float dt)
{
    UI::LateUpdate(dt);
    return 0;
}

void UIPlayerInfo::Collision(float dt)
{
    UI::Collision(dt);
}

void UIPlayerInfo::Draw(HDC hdc, float dt)
{
    UI::Draw(hdc, dt);

	m_pPlayerMPPanel->Draw(hdc, dt);

	// 체력 바
	Pos tPos = m_pPlayerMPPanel->GetPos();
	float ratio = PLAYER->GetMPRemainRatio();
	RECT rc = {};
	rc.left = tPos.x + m_iMPPanelStartX;
	rc.top = tPos.y + m_iMPPanelStartY + int((1 - ratio) * m_iMPPanelSizeY);
	rc.right = rc.left + m_iMPPanelSizeX;
	rc.bottom = rc.top + int(ratio * m_iMPPanelSizeY);

	COLORREF blend = RGB(max(int((1 - ratio) * 255.f), 10), max(int((ratio) * 255.f), 10), 0);
	DrawColorRectWithOutBorder(hdc, rc, blend);

	Pos tOffset = GetPos();
	tOffset.x += m_iItemListOffsetX;
	tOffset.y += m_iItemListOffsetY;

	int itemSelect = PLAYER->GetCurItemSel() % 12;
	DrawRedRect(hdc, MakeRect(tOffset.x + itemSelect * (m_iItemListMargin + 56.f), tOffset.y, 56, 56));

	const auto& itemList = PLAYER->AccessItemList();
	int size = min(m_iItemListOffset + 12, (int)itemList.size());
	for (int i = m_iItemListOffset; i < size; ++i)
	{
		float itemImgMargin = (56.f - itemList[i]->GetSize().x) / 2.f;
		itemList[i]->DrawImageAt(hdc, tOffset.x + itemImgMargin, tOffset.y + itemImgMargin, true);

		// 아이템 갯수 draw
		if (!itemList[i]->IsToolItem())
		{
			int num = itemList[i]->GetItemNum();
			int st_x = tOffset.x + 56.f - m_fSmallNumberSize;
			int st_y = tOffset.y + 56.f - m_fSmallNumberSize;
			while (num > 0)
			{
				m_vecSmallNumbers[num % 10]->DrawImageAt(hdc, st_x, st_y);
				num /= 10;
				st_x -= m_fSmallNumberSize;
			}
		}
		tOffset.x += m_iItemListMargin + 56.f;
	}

}
