#include "Plant.h"
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
void Plant::Grow()
{
    if (!IsDie() && m_iCurLevel < m_iMaxLevel)
    {
        if (GAMEWORLDTIME - m_iGrowTime >= m_iGrowPeriod)
        {
            m_iGrowTime = GAMEWORLDTIME;
            ++m_iCurLevel;
            if (m_iCurLevel == m_iMaxLevel)
            {
                if (!m_bPeriodic)
                {
                    ItemDrop(1, false);
                    Die();
                }
            }
            else 
            {
                GrowAsNextPlant();
            }
        }
    }
}

Plant::Plant()
    :m_iGrowTime(GAMEWORLDTIME)
{
}

Plant::~Plant()
{

}

void Plant::Die()
{
    Ref::Die();
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
