#include "GameManager.h"
#include "UIPanel.h"
#include "../../Application/Window.h"
#include "../../Core/Input.h"
#include "../../Resources/Texture.h"
#include "../../Collider/ColliderRect.h"
#include "../Object.h"
#include "../MoveObj/Player.h"
#include "../Item/Item.h"
#include "../StaticObj/Plant.h"

DEFINITION_SINGLE(GameManager);

bool GameManager::Init()
{
	INPUT->AddKey("PlayerUI", 'E');
	return true;
}

void GameManager::Input(float dt)
{
	if (KEYDOWN("PlayerUI"))
	{
		m_bPlayerInfoSelect = !m_bPlayerInfoSelect;
	}
	m_clockPanel->Input(dt);
	if (m_bPlayerInfoSelect)
	{
		m_playerInfoPanel->Input(dt);
	}
	else
	{
		m_fastItemPanel->Input(dt);
	}
	if (m_bSeedStoreSelect)
	{
		m_storePanel->Input(dt);
	}
}

int GameManager::Update(float dt)
{
	if (m_bTickStart)
	{
		m_clockPanel->Update(dt);

		// 식물 성장
		if (m_clockPanel->GetTicked())
		{
			UpdateFarm();
		}
	}

	if (m_bPlayerInfoSelect)
	{
		m_playerInfoPanel->Update(dt);
	}
	else
	{
		m_fastItemPanel->Update(dt);
	}

	if (m_bSeedStoreSelect)
	{
		m_storePanel->Update(dt);
	}

	return 0;
}

int GameManager::LateUpdate(float dt)
{
	m_clockPanel->LateUpdate(dt);
	if (m_bPlayerInfoSelect)
	{
		m_playerInfoPanel->LateUpdate(dt);
	}
	else
	{
		m_fastItemPanel->LateUpdate( dt);
	}
	if (m_bSeedStoreSelect)
	{
		m_storePanel->LateUpdate(dt);
	}
    return 0;
}

void GameManager::Collision(float dt)
{
}

void GameManager::Draw(HDC hdc, float dt)
{
	m_clockPanel->Draw(hdc, dt);
	if (m_bPlayerInfoSelect)
	{
		m_playerInfoPanel->Draw(hdc, dt);
	}
	else
	{
		m_fastItemPanel->Draw(hdc, dt);
	}
	if (m_bSeedStoreSelect)
	{
		m_storePanel->Draw(hdc, dt);
	}
}

void GameManager::AddWateredTile(int index)
{
	bool exist = false;
	for (auto& tile : m_queWateredTile)
	{
		if (tile.GetIndex() == index)
		{
			exist = true;
			tile.Reset();
			break;
		}
	}
	if(!exist)
	{
		m_queWateredTile.emplace_back(index);
	}

	m_setWateredTileIndices.insert(index);
}

vector<int> GameManager::GetDroughtTiles()
{
	vector<int> vecDroughtTiles;
	while (!m_queWateredTile.empty())
	{
		const auto & tile = m_queWateredTile.front();
		if (tile.IsDrought())
		{
			int index = tile.GetIndex();
			vecDroughtTiles.push_back(index);
			m_setWateredTileIndices.erase(index);
			m_queWateredTile.pop_front();
		}
		else
		{
			break;
		}
	}
	return vecDroughtTiles;
}

void GameManager::UpdateFarm()
{
	int tileNum = m_queWateredTile.size();
	for (int i = 0; i < tileNum; ++i)
	{
		if (!m_queWateredTile[i].IsDrought())
		{
			m_queWateredTile[i].Update();
		}
	}

	list<Plant*>::iterator iter;
	list<Plant*>::iterator iterEnd = m_plantList.end();
	for (iter = m_plantList.begin(); iter != iterEnd;)
	{
		int index = (*iter)->GetTileIndex();
		if (IsWateredTile(index))
		{
			(*iter)->Grow();
		}
		else {
			// 말라 죽는 메커니즘 구현
		}
		if ((*iter)->IsDie())
		{
			SAFE_RELEASE((*iter));
			iter = m_plantList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void GameManager::AddPlantList(Plant* pPlant)
{
	pPlant->AddRef();
	m_plantList.push_back(pPlant);
}

void GameManager::SetSeedStore(bool bSelect)
{
	m_bSeedStoreSelect = bSelect;
	if (m_bSeedStoreSelect)
	{
		Pos tPos = m_storePanel->GetPos();
		tPos.y += m_storePanel->GetSize().y;
		tPos.y -= m_clockPanel->GetSize().y - 70.f;
		m_clockPanel->SetPos(tPos);
		m_storePanel->SetClickDelay();
		m_pPlayer->DisableMovement();
	}
	else {
		m_clockPanel->SetNormalPos();
		m_pPlayer->EnableMovement();
	}
}

void GameManager::GameTimerTick()
{
	m_clockPanel->GameTimerTick();
	UpdateFarm();
}

void GameManager::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	if (m_pPlayer)
		m_pPlayer->AddRef();
}

unsigned long long GameManager::GetWorldTime() const
{
	return m_clockPanel->GetWorldTime();
}

float GameManager::GetDayDarkNess() const
{
	if (!m_bTickStart) return 0.0f;
	return m_clockPanel->GetDayDarkNess(); 
}

void GameManager::SleepUntilMorning()
{
	m_pPlayer->Sleep();
	while (!m_clockPanel->IsMorning())
	{
		GameTimerTick();
	}
	GameTimerTick();
}


GameManager::GameManager()
{

}

GameManager::~GameManager()
{
	SAFE_RELEASE(m_pPlayer);
	Safe_Release_VecList(m_plantList);

	Item::ClearItemMap();
	Plant::ClearPlantMap();
}

void GameManager::WateredTile::Reset()
{
	m_iWaterTime = GAMEWORLDTIME;
}

GameManager::WateredTile::WateredTile(int index)
	: m_iIndex(index), m_iWaterTime(GAMEWORLDTIME)
{
}

void GameManager::WateredTile::Update()
{
	if (GAMEWORLDTIME - m_iWaterTime >= m_iDroughtDuration)
		m_bDrought = true;
}