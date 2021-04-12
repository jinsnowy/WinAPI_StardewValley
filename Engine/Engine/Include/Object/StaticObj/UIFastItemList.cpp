#include "UIFastItemList.h"
#include "GameManager.h"
#include "../../Application/Window.h"
#include "../MoveObj/Player.h"
#include "../../Core/Input.h"
#include "../../Resources/ResourceManager.h"
#include "../../Collider/ColliderPoint.h"
#include "../../Collider/CollisionManager.h"
#include "../../Sound/SoundManager.h"
#include "UIPanel.h"

UIFastItemList::UIFastItemList()
{
	Init();
}

UIFastItemList::~UIFastItemList()
{
	Safe_Release_VecList(m_vecSmallNumbers);
	SAFE_RELEASE(m_pItemInfo);
}

void UIFastItemList::SellItem()
{
	if (KEYDOWN("MouseRButton"))
	{
		Pos tScreenPos = MOUSECLIENTPOS;
		Pos tOffset = GetPos();
		const auto& itemList = PLAYER->AccessItemList();
		int size = m_iItemListOffset + 12;
		for (int i = m_iItemListOffset; i < size; ++i)
		{
			if (itemList[i])
			{
				if (tScreenPos.x >= tOffset.x && tScreenPos.x < tOffset.x + m_iItemBlockSize
					&& tScreenPos.y >= tOffset.y && tScreenPos.y < tOffset.y + m_iItemBlockSize)
				{
					PLAYER->SellItem(i);
				}
			}
			tOffset.x += m_iItemListMargin + m_iItemBlockSize;
		}
	}
}

bool UIFastItemList::Init()
{
    SetTexture("FastItemList", L"SV/Scene/UI/FastItemList.bmp");
    SetColorKey(255, 255, 255);
    SetAsTextureSize();
    SetPos(float(GETRESOLUTION.x / 2) - GetSize().x / 2.f,
        float(GETRESOLUTION.y) - GetSize().y);

	COLORREF chromaKey = RGB(255, 255, 255);
	m_vecSmallNumbers = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/SmallNumbers/", chromaKey);
	m_pItemInfo = Object::CreateObject<UIPanel>("InfoPanel");
	m_pItemInfo->SetTexture("InfoPanel", L"Sv/Scene/UI/ItemInfoPanel.bmp");
	m_pItemInfo->SetAsTextureSize();
	m_pItemInfo->SetColorKey(255, 255, 255);

	return true;
}

void UIFastItemList::Input(float dt)
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
			if (m_pClickItem)
			{
				int index = GetClickIndex(MOUSECLIENTPOS);
				PLAYER->SwapItem(m_iClickItemIndex, index);
				PLAYER->SetCurItemSel(m_iClickItemIndex);
				Item* pClone = m_pClickItem->Clone();
				pClone->SetPos(MOUSEWORLDPOS);
				ColliderPoint* pPoint = pClone->AddCollider<ColliderPoint>("ItemPointBody");
				SAFE_RELEASE(pPoint);

				// 임시 콜라이더 오브젝트 추가
				COLLISION_MANAGER->AddCollideObject(pClone);
				SAFE_RELEASE(m_pClickItem);
			}
		}
	}
}

int UIFastItemList::Update(float dt)
{
    UI::Update(dt);
    return 0;
}

int UIFastItemList::LateUpdate(float dt)
{
    UI::LateUpdate(dt);
    return 0;
}

void UIFastItemList::Collision(float dt)
{
    UI::Collision(dt);
}

void UIFastItemList::Draw(HDC hdc, float dt)
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

	int itemSelect = PLAYER->GetCurItemSel() % 12;
	DrawRedRect(hdc, MakeRect(tOffset.x + itemSelect * (m_iItemListMargin + m_iItemBlockSize), tOffset.y, m_iItemBlockSize, m_iItemBlockSize));

	const auto& itemList = PLAYER->AccessItemList();
	int size = m_iItemListOffset + 12;
	for (int i = m_iItemListOffset; i < size; ++i)
	{
		if (itemList[i])
		{
			float itemImgMargin = (m_iItemBlockSize - itemList[i]->GetSize().x) / 2.f;
			itemList[i]->DrawImageAt(hdc, tOffset.x + itemImgMargin, tOffset.y + itemImgMargin, true);

			// 아이템 갯수 draw
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

	// 아이템 안내 패널
	int index = GetClickIndex(MOUSECLIENTPOS);
	Item* pItem = PLAYER->GetItem(index);
	if (pItem)
	{
		Size tSize = m_pItemInfo->GetSize();
		int px = min(MOUSECLIENTPOS.x + 35, GETRESOLUTION.x - tSize.x);
		int py = min(MOUSECLIENTPOS.y, GETRESOLUTION.y - tSize.y);
		m_pItemInfo->SetPos(px, py);
		m_pItemInfo->Draw(hdc, dt);

		const string& itemTag = pItem->GetTag();
		RESOURCE_MANAGER->DrawFontsAtFixedSize(hdc, itemTag, px + 20, py + 25, 20, 24, true, LEFT);
		RESOURCE_MANAGER->DrawFontsAtFixedSize(hdc, "Selling Price :", px + 20, py + 125, 20, 24, true, LEFT);
		RESOURCE_MANAGER->DrawFontsAtFixedSize(hdc, to_string(pItem->GetItemSellPrice()), px + 20, py + 155, 20, 24, true, LEFT);
		SAFE_RELEASE(pItem);
	}
}

int UIFastItemList::GetClickIndex(const Pos& screenPos)
{
	Pos tOffset = GetPos();
	tOffset.x += m_iItemListOffsetX;
	tOffset.y += m_iItemListOffsetY;

	const auto& itemList = PLAYER->AccessItemList();
	int size = m_iItemListOffset + 12;
	for (int i = m_iItemListOffset; i < size; ++i)
	{
		if (screenPos.x >= tOffset.x && screenPos.x < tOffset.x + m_iItemBlockSize
			&& screenPos.y >= tOffset.y && screenPos.y < tOffset.y + m_iItemBlockSize)
		{
			return i;
		}
		tOffset.x += m_iItemListMargin + m_iItemBlockSize;
	}

	return -1;
}
