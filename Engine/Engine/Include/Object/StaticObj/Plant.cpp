#include "Plant.h"
#include "../../Collider/ColliderRect.h"
#include "../../Core/PathManager.h"
#include "../../Resources/Texture.h"
#include "../../Resources/ResourceManager.h"
#include "../StaticObj/GameManager.h"

unordered_map<string, vector<Texture*>> Plant::m_mapGrowTexture;

void Plant::LoadGrowTexture(const string& plantTag, const wchar_t* folderPath, const string& strPathKey)
{
    if (!IsExist(plantTag))
    {
        vector<Texture*> vecGrowTexture =
            RESOURCE_MANAGER->LoadTextureFromDirectory(folderPath,
                RGB(255, 255, 255),
                strPathKey);
        sort(vecGrowTexture.begin(), vecGrowTexture.end(), Texture::TexSort);

        m_mapGrowTexture.insert(make_pair(plantTag, vecGrowTexture));
    }
}
bool Plant::IsExist(const string& plantTag)
{
    return m_mapGrowTexture.find(plantTag) != m_mapGrowTexture.end();
}

void Plant::ClearPlantMap()
{
    for (auto& iter : m_mapGrowTexture)
    {
        Safe_Release_VecList(iter.second);
    }
    m_mapGrowTexture.clear();
}

void Plant::GrowAsNextPlant()
{
    assert(m_GrowTexture);
    SetTexture((*m_GrowTexture)[m_iCurLevel]);
    SetAsTextureSize();
    SetPivot(0.f, (GetSize().y - TILESIZE) / GetSize().y);
}

void Plant::SetGrowTexture(const string& plantTag)
{
    if (IsExist(plantTag))
    {
        m_GrowTexture = &m_mapGrowTexture[plantTag];
        m_iMaxLevel = (int)m_GrowTexture->size();
        m_iCurLevel = 0;
        GrowAsNextPlant();
    }
}

void Plant::AddHarvestCollider()
{
    Collider* pColl = GetCollider("HarvestBody");
    if (pColl)
    {
        pColl->SetEnable(true);
        return;
    }
    ColliderRect* pRC = AddCollider<ColliderRect>("HarvestBody");
    pRC->SetRect(5.f, 5.f, TILESIZE-5.f, TILESIZE-5.f);
    pRC->AddCollisionFunction(CS_ENTER, this, &Plant::PlantHit);
    pRC->AddCollisionFunction(CS_STAY, this, &Plant::PlantHit);
    SAFE_RELEASE(pRC);
}

bool Plant::DieCondition() const
{
    return m_iHarvestNum == 0;
}

void Plant::HarvestFruit()
{
    --m_iHarvestNum;
    if (m_bPeriodic && m_iHarvestNum > 0)
    {
        ItemDrop(m_iDropItemNum);
    }
    CheckDie();
}

void Plant::PlantHit(Collider* pSrc, Collider* pDst, float dt)
{
    if (pDst->GetTag() == "SwingTool" || pDst->GetTag() == "Harvest")
    {
        if (m_iCurLevel == m_iMaxLevel - 2)
        {
            ++m_iCurLevel;
            m_iGrowTime = GAMEWORLDTIME;

            HarvestFruit();
            GrowAsNextPlant();
        }
    }
}

void Plant::Grow()
{
    if (GAMEWORLDTIME - m_iGrowTime >= m_iGrowPeriod)
    {
        m_iGrowTime = GAMEWORLDTIME;
        if (m_bPeriodic)
        {
            if (m_iCurLevel < m_iMaxLevel - 2)
            {
                ++m_iCurLevel;
                GrowAsNextPlant();
                // 열매 있는 상태
                if (m_iCurLevel == m_iMaxLevel - 2)
                {
                    AddHarvestCollider();
                }
            }
            // 열매 없는 상태
            else if (m_iCurLevel == m_iMaxLevel - 1)
            {
                --m_iCurLevel;
                GrowAsNextPlant();
                AddHarvestCollider();
            }
        }
        else
        {
            ++m_iCurLevel;
            GrowAsNextPlant();
            if (m_iCurLevel == m_iMaxLevel - 1)
            {
                HarvestFruit();
            }
        }
    }
}

void Plant::Peek()
{
    m_iGrowTime = GAMEWORLDTIME;
}

Plant::Plant()
    :m_iGrowTime(GAMEWORLDTIME)
{
}

Plant::~Plant()
{

}

bool Plant::Init()
{
    return true;
}

void Plant::Input(float dt)
{
    InteractiveTile::Input(dt);
}

int Plant::Update(float dt)
{
    InteractiveTile::Update(dt);
    return 0;
}

void Plant::AfterDie()
{
   ItemDrop(m_iDropItemNum, false);
}

int Plant::LateUpdate(float dt)
{
    InteractiveTile::LateUpdate(dt);
    return 0;
}

void Plant::Collision(float dt)
{
    InteractiveTile::Collision(dt);
}

void Plant::Draw(HDC hDC, float dt)
{
    InteractiveTile::Draw(hDC, dt);
}

void Plant::Save(FILE* pFile)
{
    InteractiveTile::Save(pFile);
}

void Plant::Load(FILE* pFile)
{
    InteractiveTile::Load(pFile);
}
