#include "CropCabinet.h"
#include "../../Collider/ColliderRect.h"
#include "GameManager.h"
#include "../MoveObj/Player.h"

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
        PLAYER->SellItem(pItem->GetTag());
    }
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
