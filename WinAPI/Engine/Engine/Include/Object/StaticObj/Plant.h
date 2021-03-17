#pragma once
#include "InteractiveTile.h"

class Plant :
	public InteractiveTile
{
	friend class Object;
	friend class GameScene;
	friend class GameManager;
private:
	vector<Texture*>* m_GrowTexture = nullptr;
	static unordered_map<string, vector<Texture*>> m_mapGrowTexture;
	static void ClearPlantMap();
protected:
	bool m_bPeriodic = false;
	int m_iGrowPeriod = 0;
	int m_iCurLevel = 0;
	int m_iMaxLevel = 0;
	int m_iTileIndex = 0;
	int m_iDroughtDuration = 0;
	unsigned long long m_iGrowTime;
	static bool IsExist(const string& plantTag);
	static void LoadGrowTexture(const string& plantTag, const wchar_t* folderPath, const string& strPathKey = TEXTURE_PATH);
	void SetGrowTexture(const string& plantTag);
	void SetPeriodicPlant() { m_bPeriodic = true; }
	void SetGrowPeriod(int period) { m_iGrowPeriod = period; }
	void SetTileIndex(int index) { m_iTileIndex = index; }
private:
	void GrowAsNextPlant();
public:
	int GetTileIndex() const { return m_iTileIndex; }
	int GetCurLevel() const { return m_iCurLevel; }
	void Grow();
protected:
	Plant();
	Plant(const Plant& obj) = delete;
	virtual ~Plant();
	virtual void TileHit(Collider* pSrc, Collider* pDst, float dt) = 0;
	virtual void Die() = 0;
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual Plant* Clone() { throw EXCEPT(L"Plant cloned !"); return nullptr; };
	virtual void LateInit() = 0;
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};