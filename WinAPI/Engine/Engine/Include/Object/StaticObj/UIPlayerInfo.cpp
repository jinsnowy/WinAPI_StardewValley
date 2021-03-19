#include "UIPlayerInfo.h"
#include "GameManager.h"
#include "../../Application/Window.h"
#include "../MoveObj/Player.h"
#include "../../Core/Input.h"
#include "../../Resources/ResourceManager.h"
#include "../../Resources/Texture.h"

UIPlayerInfo::UIPlayerInfo()
{
	Init();
}

UIPlayerInfo::~UIPlayerInfo()
{
	Safe_Release_VecList(m_vecSmallNumbers);
	SAFE_RELEASE(m_pClickItem);
}

bool UIPlayerInfo::Init()
{
	SetTexture("UIPlayerInfo", L"SV/Scene/UI/PlayerInfoUI.bmp");
	SetAsTextureSize();
	SetColorKey(255, 255, 255);
	Size tSize = GetSize();
	SetPos((float(GETRESOLUTION.x) - tSize.x )/ 2.f,
			(float(GETRESOLUTION.y) - tSize.y)/2.f);
	COLORREF chromaKey = RGB(255, 255, 255);
	m_vecSmallNumbers = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/SmallNumbers/", chromaKey);
	return true;
}

void UIPlayerInfo::Input(float dt)
{
	UI::Input(dt);

	if (KEYDOWN("MouseLButton"))
	{
		SAFE_RELEASE(m_pClickItem);
		m_iClickItemIndex = GetClickIndex(MOUSECLIENTPOS);
		m_pClickItem = PLAYER->GetItem(m_iClickItemIndex);
		PLAYER->SetCurItemSel(m_iClickItemIndex);
	}
	if (KEYPRESS("MouseLButton"))
	{
		if (!m_bDrag)
		{
			m_bDrag = true;
		}
	}
	if (KEYUP("MouseLButton"))
	{
		if (m_bDrag)
		{
			m_bDrag = false;
			int index = GetClickIndex(MOUSECLIENTPOS);
			PLAYER->SwapItem(m_iClickItemIndex, index);
			SAFE_RELEASE(m_pClickItem);
		}
	}
}

int UIPlayerInfo::Update(float dt)
{
	UI::Update(dt);
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

	if (m_bDrag && m_pClickItem)
	{
		Pos tPos = MOUSECLIENTPOS;
		tPos -= Pos(m_pClickItem->GetImageSize().x / 2, m_pClickItem->GetImageSize().y / 2);
		m_pClickItem->DrawImageAt(hdc, tPos);
	}

	Pos tOffset = GetPos();
	tOffset.x += m_iItemListOffsetX;
	tOffset.y += m_iItemListOffsetY;

	int startX = tOffset.x;
	int itemSelect = PLAYER->GetCurItemSel() % 12;
	DrawRedRect(hdc, MakeRect(tOffset.x + itemSelect * (m_iItemListMargin + m_iItemBlockSize), tOffset.y, m_iItemBlockSize, m_iItemBlockSize));

	const auto& itemList = PLAYER->AccessItemList();

	for (int i = 0; i < 36; ++i)
	{
		if (i % 12 == 0)
		{
			tOffset.x = startX;
			if (i == 12)
			{
				tOffset.y += 90.f;
			}
			else if(i == 24) {
				tOffset.y += 70.f;
			}
		}
		if (itemList[i])
		{
			float itemImgMargin = (m_iItemBlockSize - itemList[i]->GetSize().x) / 2.f;
			itemList[i]->DrawImageAt(hdc, tOffset.x + itemImgMargin, tOffset.y + itemImgMargin, true);

			// ¾ÆÀÌÅÛ °¹¼ö draw
			if (!itemList[i]->IsToolItem())
			{
				int num = itemList[i]->GetItemNum();
				int st_x = tOffset.x + m_iItemBlockSize - m_fSmallNumberSize;
				int st_y = tOffset.y + m_iItemBlockSize - m_fSmallNumberSize;
				while (num > 0)
				{
					m_vecSmallNumbers[num % 10]->DrawImageAt(hdc, st_x, st_y);
					num /= 10;
					st_x -= m_fSmallNumberSize;
				}
			}	
		}
		tOffset.x += m_iItemListMargin + m_iItemBlockSize;
	}
}

int UIPlayerInfo::GetClickIndex(const Pos& screenPos)
{
	Pos tOffset = GetPos();
	tOffset.x += m_iItemListOffsetX;
	tOffset.y += m_iItemListOffsetY;
	int startX = tOffset.x;
	const auto& itemList = PLAYER->AccessItemList();
	for (int i = 0; i < 36; ++i)
	{
		if (i % 12 == 0)
		{
			tOffset.x = startX;
			if (i == 12)
			{
				tOffset.y += 90.f;
			}
			else if (i == 24) {
				tOffset.y += 70.f;
			}
		}
		if (screenPos.x >= tOffset.x && screenPos.x < tOffset.x + m_iItemBlockSize
			&& screenPos.y >= tOffset.y && screenPos.y < tOffset.y + m_iItemBlockSize)
		{
			return i;
		}
		tOffset.x += m_iItemListMargin + m_iItemBlockSize;

	}
	return -1;
}
