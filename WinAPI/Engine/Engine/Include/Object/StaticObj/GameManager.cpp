#include "GameManager.h"
#include "UIGameTimer.h"
#include "UISeedStore.h"
#include "UIFastItemList.h"
#include "UIPlayerInfo.h"
#include "UIPanel.h"
#include "../../Application/Window.h"
#include "../../Core/Input.h"
#include "../../Resources/Texture.h"
#include "../../Collider/ColliderRect.h"
#include "../Object.h"
#include "../MoveObj/Player.h"
#include "../Item/Item.h"
#include "../StaticObj/Plant.h"
#include "../../Sound/SoundManager.h"

DEFINITION_SINGLE(GameManager);

bool GameManager::Init()
{
	INPUT->AddKey("PlayerUI", 'E');
	SOUND_MANAGER->LoadSound("OpenUI", false, SD_EFFECT, "OpenUI.mp3");
	SOUND_MANAGER->LoadSound("CloseUI", false, SD_EFFECT, "CloseUI.mp3");
	SOUND_MANAGER->LoadSound("InteractUI", false, SD_EFFECT, "InteractUI.mp3");

	m_uiPanels.resize((int)PANEL_TYPE::PANEL_END);
	m_uiPanels[(int)PANEL_TYPE::PLAYER_INFO] = make_unique<UIPlayerInfo>();
	m_uiPanels[(int)PANEL_TYPE::GAME_TIMER] = make_unique<UIGameTimer>();
	m_uiPanels[(int)PANEL_TYPE::FAST_ITEMLIST] = make_unique<UIFastItemList>();
	m_uiPanels[(int)PANEL_TYPE::SEED_STORE] = make_unique<UISeedStore>();
	m_uiStates.reset();
	return true;
}

void GameManager::Start()
{
	m_uiStates[(int)PANEL_TYPE::GAME_TIMER] = true;
	m_uiStates[(int)PANEL_TYPE::FAST_ITEMLIST] = true;
}

void GameManager::Input(float dt)
{
	if (KEYDOWN("PlayerUI"))
	{
		m_uiStates.flip((int)PANEL_TYPE::PLAYER_INFO);
		m_uiStates.flip((int)PANEL_TYPE::FAST_ITEMLIST);
		Activated((int)PANEL_TYPE::PLAYER_INFO) ? 
			SOUND_MANAGER->PlaySound("OpenUI") : SOUND_MANAGER->PlaySound("CloseUI");
	}
	for (int i = 0; i < (int)PANEL_TYPE::PANEL_END; ++i)
	{
		if (Activated(i))
		{
			m_uiPanels[i]->Input(dt);
		}
	}
}

int GameManager::Update(float dt)
{
	for (int i = 0; i < (int)PANEL_TYPE::PANEL_END; ++i)
	{
		if (Activated(i))
		{
			m_uiPanels[i]->Update(dt);

			if (i == (int)PANEL_TYPE::GAME_TIMER)
			{
				if (((UIGameTimer*)m_uiPanels[i].get())->GetTicked())
				{
					UpdateFarm();
				}
			}
		}
	}
	return 0;
}

int GameManager::LateUpdate(float dt)
{
	for (int i = 0; i < (int)PANEL_TYPE::PANEL_END; ++i)
	{
		if (Activated(i))
		{
			m_uiPanels[i]->LateUpdate(dt);
		}
	}
    return 0;
}

void GameManager::Collision(float dt)
{
	for (int i = 0; i < (int)PANEL_TYPE::PANEL_END; ++i)
	{
		if (Activated(i))
		{
			m_uiPanels[i]->Collision(dt);
		}
	}
}

void GameManager::Draw(HDC hdc, float dt)
{
	for (int i = 0; i < (int)PANEL_TYPE::PANEL_END; ++i)
	{
		if (Activated(i))
		{
			m_uiPanels[i]->Draw(hdc, dt);
		}
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

		if (!(*iter)->GetLife())
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
	m_uiStates[(int)PANEL_TYPE::SEED_STORE] = bSelect;
	UIGameTimer* pTimer = static_cast<UIGameTimer*>(m_uiPanels[(int)PANEL_TYPE::GAME_TIMER].get());
	UISeedStore* pStore = static_cast<UISeedStore*>(m_uiPanels[(int)PANEL_TYPE::SEED_STORE].get());
	if (bSelect)
	{
		Pos tPos = pTimer->GetPos();
		tPos.y -= pTimer->GetSize().y - 70.f;
		tPos.y += pStore->GetSize().y;
		pTimer->SetPos(tPos);
		pStore->SetClickDelay();
		m_pPlayer->DisableMovement();
		SOUND_MANAGER->PlaySound("OpenUI");
	}
	else {
		pTimer->SetNormalPos();
		m_pPlayer->EnableMovement();
		SOUND_MANAGER->PlaySound("CloseUI");
	}
}

void GameManager::GameTimerTick()
{
	static_cast<UIGameTimer*>(m_uiPanels[(int)PANEL_TYPE::GAME_TIMER].get())->GameTimerTick();
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
	return static_cast<UIGameTimer*>(m_uiPanels[(int)PANEL_TYPE::GAME_TIMER].get())->GetWorldTime();
}

float GameManager::GetDayDarkNess() const
{
	if (!m_uiPanels[(int)PANEL_TYPE::GAME_TIMER]) return 0.0f;
	return static_cast<UIGameTimer*>(m_uiPanels[(int)PANEL_TYPE::GAME_TIMER].get())->GetDayDarkNess();
}

void GameManager::SleepUntilMorning()
{
	m_pPlayer->Sleep();
	UIGameTimer* pTimer = static_cast<UIGameTimer*>(m_uiPanels[(int)PANEL_TYPE::GAME_TIMER].get());
	while (!pTimer->IsMorning())
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