#include "UIFastItemList.h"
#include "GameManager.h"
#include "../../Application/Window.h"
#include "../MoveObj/Player.h"
#include "../../Core/Input.h"
#include "../../Resources/ResourceManager.h"


UIFastItemList::UIFastItemList()
{
	Init();
}

UIFastItemList::~UIFastItemList()
{
	Safe_Release_VecList(m_vecSmallNumbers);
	
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
	INPUT->AddKey("NextItemList", VK_TAB);

	return true;
}

void UIFastItemList::Input(float dt)
{
    UI::Input(dt);
	if (KEYDOWN("NextItemList"))
	{
		m_iItemListOffset = (m_iItemListOffset + 12) % 36;
		PLAYER->SetCurItemSel((PLAYER->GetCurItemSel() + 12) % 36);
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

	Pos tOffset = GetPos();
	tOffset.x += m_iItemListOffsetX;
	tOffset.y += m_iItemListOffsetY;

	int itemSelect = PLAYER->GetCurItemSel() % 12;
	DrawRedRect(hdc, MakeRect(tOffset.x + itemSelect * (m_iItemListMargin + 56.f), tOffset.y, 56, 56));

	const auto& itemList = PLAYER->AccessItemList();
	int size = m_iItemListOffset + 12;
	for (int i = m_iItemListOffset; i < size; ++i)
	{
		if (itemList[i])
		{
			float itemImgMargin = (56.f - itemList[i]->GetSize().x) / 2.f;
			itemList[i]->DrawImageAt(hdc, tOffset.x + itemImgMargin, tOffset.y + itemImgMargin, true);

			// ¾ÆÀÌÅÛ °¹¼ö draw
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
		}	
		tOffset.x += m_iItemListMargin + 56.f;
	}

}
