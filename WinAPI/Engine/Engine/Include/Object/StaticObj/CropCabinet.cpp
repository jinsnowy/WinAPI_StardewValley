#include "CropCabinet.h"
#include "../../Collider/ColliderRect.h"
#include "GameManager.h"
#include "../MoveObj/Player.h"
#include "../TempObject.h"
#include "../../Resources/ResourceManager.h"
#include "../../Application/Window.h"
#include "../../Scene/Scene.h"
#include "../../Effect/VanishEffect.h"
CropCabinet::CropCabinet()
{
    m_eColliderChannel = CO_PLAYER;
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
    SetTexture("CropCabinet", L"SV/Object/CropCabinet/CropCabinet.bmp");
    SetAsTextureSize();
    SetColorKey(255, 255, 255);
    SetPivot(0.f, 1.0f);
    SetPos(m_fPosX, m_fPosY);

    ColliderRect* pRC = AddCollider<ColliderRect>("CabinetBody");
    pRC->SetRect(0.f, -GetSize().y, GetSize().x, 0.f);
    pRC->AddCollisionFunction(CS_ENTER, this, &CropCabinet::Click);
    pRC->AddCollisionFunction(CS_STAY, this, &CropCabinet::Click);
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
    if (pDst->GetTag() == "ItemPointBody")
    {
        Item* pItem = static_cast<Item*>(pDst->GetObj());
        bool bSold = PLAYER->SellItem(pItem->GetTag());

        if (bSold)
        {
            SoldEffect(pItem->GetItemSellPrice());
        }
    }
}

void CropCabinet::SoldEffect(int price)
{
    vector<int> digits;
    do
    {
        digits.push_back(price % 10);
        price /= 10;
    } while (price > 0);

    reverse(digits.begin(), digits.end());

    int NumOfDigits = (int)digits.size();
    Texture* pTex = Texture::CreateEmptyTexture(WINDOW->GetWndDC(), m_iWidth * NumOfDigits, m_iHeight, util::White);
    pTex->SetColorKey(util::White);

    int stX = 0, stY = 0;
    char baseName[10] = "Money";
    baseName[6] = '\0';
    for (int digit : digits)
    {
        baseName[5] = '0' + digit;
        Texture* pNumTex = RESOURCE_MANAGER->FindTexture(baseName);

        pTex->DrawImageFrom(stX, stY, m_iWidth, m_iHeight, pNumTex, 0, 0);
        stX += m_iWidth;

        SAFE_RELEASE(pNumTex);
    }

    TempObject* pObj = Object::CreateObject<TempObject>("SoldEffect");
    pObj->SetPos(GetRight(), GetTop());
    pObj->SetLifeDuration(1.0f);
    pObj->SetTexture(pTex);
    pObj->SetAsTextureSize();
    pObj->SetEffect(new VanishEffect(pObj, 1.0f, Pos(0.0, -30.f)));
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
