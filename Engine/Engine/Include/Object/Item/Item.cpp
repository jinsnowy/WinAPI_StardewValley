#include "Item.h"
#include "../../Collider/ColliderRect.h"
#include "../MoveObj/Player.h"
#include "../../Scene/GameScene.h"
#include "../../Scene/Scene.h"
#include "../../Core/PathManager.h"
#include "../../Effect/BoundEffect.h"
#include "../../Effect/SequentialEffect.h"
#include "../../Effect/ChaseEffect.h"
#include "../../Effect/ChaseLongEffect.h"
#include "../StaticObj/GameManager.h"

unordered_map<string, Item*> Item::m_mapItem;
normal_distribution<float> Item::m_AngleDist(m_fDropAngle, m_fDropAngleVar);
normal_distribution<float> Item::m_VeloDist(m_fDropVelo, m_fDropVeloVar);

Item* Item::FindItem(const string& strItemKey)
{
    auto found = m_mapItem.find(strItemKey);
    if (found == m_mapItem.end())
        return nullptr;
    found->second->AddRef();
    return found->second;
}

void Item::GenerateItemEffect()
{
    float angle = m_AngleDist(util::_rng);
    float velo = m_VeloDist(util::_rng);

    angle = (rand() % 2 == 1) ? angle : PI - angle;

    EffectPtr pBound = make_shared<BoundEffect>(this, 2.5f, 4, angle, velo, GetPos().y + 0.5f);
    EffectPtr pChase;
    if (PLAYER->HasRing())
    {
        pChase = make_shared<ChaseLongEffect>(this, PLAYER);
    }
    else
    {
        pChase = make_shared<ChaseEffect>(this, PLAYER);
    }
    EffectPtr pSeq = make_shared<SequentialEffect>(initializer_list<EffectPtr> { pBound, pChase });
    SetEffect(pSeq);
}

Item::Item()
{
}

Item::Item(const Item& item)
    : Object(item)
{
    m_iItemNum = 1;
    m_iSellPrice = item.m_iSellPrice;
    m_iPrice = item.m_iPrice;
    m_eItemType = item.m_eItemType;
}

Item::~Item()
{
}

void Item::Decrease()
{
    --m_iItemNum; 
    if (m_iItemNum == 0)
    {
        Die();
    }
}

bool Item::Init()
{
    ColliderRect* pColl =  AddCollider<ColliderRect>("ItemBody");
    pColl->SetRect(10.f, 10.f, 30.f, 30.f);
    SAFE_RELEASE(pColl);
    return true;
}

void Item::Input(float dt)
{
    Object::Input(dt);
}

int Item::Update(float dt)
{
    Object::Update(dt);

    return 0;
}

int Item::LateUpdate(float dt)
{
    Object::LateUpdate(dt);
    return 0;
}

void Item::Collision(float dt)
{
    Object::Collision(dt);
}

void Item::Draw(HDC hdc, float dt)
{
    Object::Draw(hdc, dt);
}

Item* Item::Clone()
{
    return new Item(*this);
}
