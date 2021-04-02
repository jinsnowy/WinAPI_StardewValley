#include "CropCabinet.h"
#include "../../Collider/ColliderRect.h"
#include "GameManager.h"
#include "../MoveObj/Player.h"
#include "../TempObject.h"
#include "../../Resources/ResourceManager.h"
#include "../../Application/Window.h"
#include "../../Scene/Scene.h"
#include "../../Effect/VanishEffect.h"
#include "../../Core/Input.h"
#include "../../Sound/SoundManager.h"
CropCabinet::CropCabinet()
{
}

CropCabinet::CropCabinet(const CropCabinet& obj)
    : StaticObject(obj)
{
}

CropCabinet::~CropCabinet()
{
}

bool CropCabinet::Init()
{
    SOUND_MANAGER->LoadSound("CabinetOpen", false, SD_EFFECT, "Object/CabinetOpen.mp3");
    SOUND_MANAGER->LoadSound("CabinetClose", false, SD_EFFECT, "Object/CabinetClose.mp3");
    SOUND_MANAGER->LoadSound("CabinetDump", false, SD_EFFECT, "Object/CabinetDump.mp3");

    SetTexture("CropCabinetOpen", L"SV/Object/CropCabinet/CropCabinetOpen.bmp");
    SetColorKey(255, 255, 255);
    SetTexture("CropCabinetClose", L"SV/Object/CropCabinet/CropCabinetClose.bmp");
    SetAsTextureSize();
    SetPivot(0.f, (GetSize().y - TILESIZE) / GetSize().y);
    SetColorKey(255, 255, 255);
    SetPos(m_fPosX, m_fPosY);

    ColliderRect* pRC = AddCollider<ColliderRect>("CabinetSellBody");
    pRC->SetRect(0.f, -TILESIZE, GetSize().x, GetSize().y - TILESIZE);
    pRC->AddCollisionFunction(CS_ENTER, this, &CropCabinet::Click);
    pRC->AddCollisionFunction(CS_STAY, this, &CropCabinet::Click);
    SAFE_RELEASE(pRC);

    pRC = AddCollider<ColliderRect>("CabinetBody");
    pRC->SetRect(-TILESIZE, -GetSize().y, GetSize().x + TILESIZE, 2 * TILESIZE);
    pRC->AddCollisionFunction(CS_ENTER, this, &CropCabinet::OnSite);
    pRC->AddCollisionFunction(CS_STAY, this, &CropCabinet::OnSite);
    pRC->AddCollisionFunction(CS_LEAVE, this, &CropCabinet::OffSite);
    SAFE_RELEASE(pRC);

    return true;
}

void CropCabinet::Input(float dt)
{
    StaticObject::Input(dt);
}

int CropCabinet::Update(float dt)
{
    StaticObject::Update(dt);
    return 0;
}

int CropCabinet::LateUpdate(float dt)
{
    StaticObject::LateUpdate(dt);
    return 0;
}

void CropCabinet::Collision(float dt)
{
    StaticObject::Collision(dt);
}

void CropCabinet::Draw(HDC hDC, float dt)
{
    StaticObject::Draw(hDC, dt);
}

CropCabinet* CropCabinet::Clone()
{
    return new CropCabinet(*this);
}

void CropCabinet::Click(Collider* pSrc, Collider* pDst, float dt)
{
    if (m_State == State::OPEN)
    {
        if (pDst->GetTag() == "ItemPointBody")
        {
            Item* pItem = static_cast<Item*>(pDst->GetObj());
            bool bSold = PLAYER->SellItem(pItem->GetTag());
            if (bSold)
            {
                SoldEffect(pItem);
            }
        }
        if (pDst->GetTag() == "Click")
        {
            Item* pItem = PLAYER->GetCurItem();
            if (pItem && !pItem->IsToolItem())
            {
                bool bSold = PLAYER->SellItem(pItem->GetTag());
                if (bSold)
                {
                    SoldEffect(pItem);
                }
                SAFE_RELEASE(pItem);
            }
        }
    }
}

void CropCabinet::OnSite(Collider* pSrc, Collider* pDst, float dt)
{
    if (pDst->GetTag() == "PlayerBody")
    {
        if (m_State == State::CLOSE)
        {
            SOUND_MANAGER->PlayMusic("CabinetOpen");
            SetTexture("CropCabinetOpen");
            SetAsTextureSize();
            SetPivot(0.f, (GetSize().y - TILESIZE) / GetSize().y);
        }
        m_State = State::OPEN;
    }
}

void CropCabinet::OffSite(Collider* pSrc, Collider* pDst, float dt)
{
    if (pDst->GetTag() == "PlayerBody")
    {
        if (m_State == State::OPEN)
        {
            SOUND_MANAGER->PlayMusic("CabinetClose");
            SetTexture("CropCabinetClose");
            SetAsTextureSize();
            SetPivot(0.f, (GetSize().y - TILESIZE) / GetSize().y);
        }
        m_State = State::CLOSE;
    }
}


void CropCabinet::SoldEffect(Item* pItem)
{
    SOUND_MANAGER->PlayMusic("CabinetDump");

    TempObject* pObj = Object::CreateObject<TempObject>("SoldItem");

    Pos tPos = GetCenter();
    tPos.y -= 30.f;
    tPos.x -= pItem->GetImageSize().x / 2.f;
    pObj->SetPos(tPos);
    pObj->SetLifeDuration(1.0f);
    pObj->SetTexture(pItem->AccessTexture());
    pObj->SetAsTextureSize();
    pObj->SetEffect(make_unique<VanishEffect>(pObj, 1.0f, Pos(0.0, -30.f)));
    m_pScene->FindLayer("Effect")->AddObject(pObj);
    SAFE_RELEASE(pObj);

    int price = pItem->GetItemSellPrice();
    vector<int> digits;
    do
    {
        digits.push_back(price % 10);
        price /= 10;
    } while (price > 0);

    reverse(digits.begin(), digits.end());

    int NumOfDigits = (int)digits.size();
    Texture* pTex = Texture::CreateEmptyTexture(WINDOW->GetWndDC(), int(m_iWidth * NumOfDigits), int(m_iHeight), util::White);
    pTex->SetColorKey(util::White);

    int stX = 0, stY = 0;
    char baseName[10] = "Money";
    baseName[6] = '\0';
    for (int digit : digits)
    {
        baseName[5] = '0' + digit;
        Texture* pNumTex = RESOURCE_MANAGER->FindTexture(baseName);

        pTex->DrawImageFrom(stX, stY, int(m_iWidth), int(m_iHeight), pNumTex, 0, 0);
        stX += int(m_iWidth);

        SAFE_RELEASE(pNumTex);
    }

    pObj = Object::CreateObject<TempObject>("SoldEffect");
    pObj->SetPos(GetRight(), GetTop());
    pObj->SetLifeDuration(1.0f);
    pObj->SetTexture(pTex);
    pObj->SetAsTextureSize();
    pObj->SetEffect(make_unique<VanishEffect>(pObj, 1.0f, Pos(0.0, -30.f)));
    m_pScene->FindLayer("Effect")->AddObject(pObj);

    SAFE_RELEASE(pTex);
    SAFE_RELEASE(pObj);
}

void CropCabinet::Save(FILE* pFile)
{
    StaticObject::Save(pFile);
}

void CropCabinet::Load(FILE* pFile)
{
    StaticObject::Load(pFile);
}

void CropCabinet::LateInit()
{

}
