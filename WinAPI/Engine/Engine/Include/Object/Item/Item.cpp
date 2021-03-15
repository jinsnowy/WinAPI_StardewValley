#include "Item.h"
#include "../../Collider/ColliderRect.h"
#include "../MoveObj/Player.h"
#include "../../Scene/GameScene.h"
#include "../../Scene/Scene.h"
#include "../../Core/PathManager.h"
#include "../../Effect/BoundEffect.h"

unordered_map<string, Item*> Item::m_mapItem;
normal_distribution<float> Item::m_AngleDist(m_fDropAngle, m_fDropAngleVar);
normal_distribution<float> Item::m_VeloDist(m_fDropVelo, m_fDropVeloVar);

Item* Item::FindItem(const string& strItemKey)
{
    auto found = m_mapItem.find(strItemKey);
    if (found == m_mapItem.end())
        return nullptr;
    return found->second;
}

vector<Item*> Item::LoadItemFromDirectory(const wchar_t* folderPath, COLORREF chromaKey, const string& strPathKey)
{
    vector<Item*> vecItem;
    const wchar_t* pPath = PATH_MANAGER->FindPath(strPathKey);
    wstring strPath;
    if (pPath)
        strPath = pPath;

    strPath += folderPath;
    assert(strPath.back() == L'\\' || strPath.back() == L'/');

    string strPathString(strPath.begin(), strPath.end());
    for (const auto& entry : fs::directory_iterator(strPath))
    {
        const wchar_t* imgPath = entry.path().c_str();
        string strItemKey = ExtractKeyFromPathString(GetChar(imgPath), lstrlen(imgPath));


        Item* pItem = FindItem(strItemKey);
        if (pItem)
        {
            pItem->AddRef();
            vecItem.push_back(pItem);
            continue;
        }

        pItem = Object::CreateObject<Item>(strItemKey);

        pItem->SetTexture(strItemKey, imgPath, "");
        pItem->SetAsTextureSize();
        pItem->SetColorKey(255, 255, 255);

        pItem->AddRef();
        m_mapItem.insert(make_pair(strItemKey, pItem));

        vecItem.push_back(pItem);
    }
    return vecItem;
}

Item* Item::CreateCloneItem(const string& strItemKey, const wchar_t* pFileName, const string& strPathKey)
{
    Item* pItem = FindItem(strItemKey);
    if (pItem)
        return pItem->Clone();

    pItem = Object::CreateObject<Item>(strItemKey);
    pItem->SetTexture(strItemKey, pFileName);
    pItem->SetAsTextureSize();
    pItem->SetColorKey(255, 255, 255);
    m_mapItem.insert(make_pair(strItemKey, pItem));

    return pItem->Clone();
}

void Item::ChasePlayer(float dt)
{
    const Pos &tPos = GetPos();
    const Pos &targetPos = static_cast<GameScene*>(m_pScene)->AccessPlayer()->GetCenter();
    if (Math::Distance(targetPos, tPos) <= m_fChaseRange)
    {
        Pos dir = (targetPos - tPos).GetNormalized();
        dir *= (m_fChaseSpeed * dt);
        SetPos(tPos.x + dir.x, tPos.y + dir.y);
    }
}

void Item::GenerateBoundEffect()
{
    assert(m_Effect == nullptr);

    float angle = m_AngleDist(util::_rng);
    float velo = m_VeloDist(util::_rng);
    
    angle = (rand() % 2 == 1) ? angle : PI - angle;

    m_Effect = new BoundEffect;
    m_Effect->Init(GetPos(), 4, angle, velo, GetPos().y + 0.5f, 2.5f);
}

Item::Item()
{
    m_eAdvertiseChannel = CO_ITEM;
    m_eListenChannel = CO_PLAYER;
}

Item::Item(const Item& item)
    : Object(item)
{
    m_iItemNum = 1;
}

Item::~Item()
{
    SAFE_DELETE(m_Effect);
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

    if (m_pScene)
    {
        if (m_Effect)
        {
            SetPos(m_Effect->Next(dt));
            if (m_Effect->IsEnd())
            {
                SAFE_DELETE(m_Effect);
            }
        }
        ChasePlayer(dt);
    }

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
