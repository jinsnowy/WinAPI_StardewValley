#include "Npc.h"
#include "../../Collider/ColliderRect.h"
#include "UIGameManager.h"

Npc::Npc()
{
    m_iObjType = OBJ_NPC;
}

Npc::Npc(const Npc& obj)
    : StaticObject(obj)
{
}

Npc::~Npc()
{
}

bool Npc::Init()
{
    SetTexture("NPC", L"SV/Object/NPC.bmp");
    SetAsTextureSize();
    SetColorKey(255, 255, 255);
    SetPivot(0.f, 1.0f);

    ColliderRect* pRC = AddCollider<ColliderRect>("NPCBody");
    pRC->SetRect(0.f, -128.f, 64.f, 0.f);
    SAFE_RELEASE(pRC);
    return true;
}

void Npc::Input(float dt)
{
    StaticObject::Input(dt);
}

int Npc::Update(float dt)
{
    StaticObject::Update(dt);
    return 0;
}

int Npc::LateUpdate(float dt)
{
    StaticObject::LateUpdate(dt);
    return 0;
}

void Npc::Collision(float dt)
{
    StaticObject::Collision(dt);
}

void Npc::Draw(HDC hDC, float dt)
{
    StaticObject::Draw(hDC, dt);
}

Npc* Npc::Clone()
{
    return new Npc(*this);
}

void Npc::Click(Collider* pSrc, Collider* pDst, float dt)
{
   if (pDst->GetTag() == "Click")
    {
        UI_MANAGER->SetSeedStore(true);
    }
}

void Npc::Save(FILE* pFile)
{
    StaticObject::Save(pFile);
}

void Npc::Load(FILE* pFile)
{
    StaticObject::Load(pFile);
}

void Npc::LateInit()
{
    AdvertiseFrom(CO_UI);
    ListenTo(CO_PLAYER);

    Collider* pRC = GetCollider("NPCBody");
    pRC->AddCollisionFunction(CS_ENTER, this, &Npc::Click);
    pRC->AddCollisionFunction(CS_STAY, this, &Npc::Click);
    SAFE_RELEASE(pRC);
}
