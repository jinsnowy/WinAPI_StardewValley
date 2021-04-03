#pragma once
#include "../Object.h"
#include "../../Core/PathManager.h"
class Item : public Object
{
	friend class Object;
	friend class GameManager;
private:
	static constexpr float m_fDropAngle = DEG2RAD(75.f);
	static constexpr float m_fDropAngleVar = DEG2RAD(10.f);
	static constexpr float m_fDropVelo = 300.f;
	static constexpr float m_fDropVeloVar = DEG2RAD(50.f);
	static normal_distribution<float> m_AngleDist;
	static normal_distribution<float> m_VeloDist;
	static unordered_map<string, Item*> m_mapItem;
private:
	static void ClearItemMap()
	{
		Safe_Release_Map(m_mapItem);
	}
public:
	static Item* FindItem(const string& strItemKey);
	template<typename T>
	static Item* CreateCloneItem(const string& strItemKey,
							const wchar_t* pFileName,
							const string& strPathKey = TEXTURE_PATH)
	{
		Item* pItem = FindItem(strItemKey);

		if (!pItem)
		{
			pItem = Object::CreateObject<T>(strItemKey);
			pItem->SetTexture(strItemKey, pFileName);
			pItem->SetAsTextureSize();
			pItem->SetColorKey(255, 255, 255);

			pItem->AddRef();
			m_mapItem.insert(make_pair(strItemKey, pItem));
			return pItem;
		}

		Item* pClone = pItem->Clone();
		SAFE_RELEASE(pItem);
		return pClone;
	}
	template<typename T>
	static vector<Item*> LoadItemFromDirectory(const wchar_t* folderPath, COLORREF chromaKey, const string& strPathKey = TEXTURE_PATH)
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
			if (!pItem)
			{
				pItem = Object::CreateObject<T>(strItemKey);

				pItem->SetTexture(strItemKey, imgPath, "");
				pItem->SetAsTextureSize();
				pItem->SetColorKey(255, 255, 255);

				pItem->AddRef();
				m_mapItem.insert(make_pair(strItemKey, pItem));
			}

			vecItem.push_back(static_cast<T*>(pItem));
		}
		return vecItem;
	}
protected:
	int m_iPrice = 0;
	int m_iSellPrice = 0;
	int m_iItemNum = 1;
	ITEM_TYPE m_eItemType = ITEM_NONE;
public:
	void GenerateItemEffect();
	bool IsSeedItem() const { return m_eItemType == ITEM_SEED; }
	bool IsToolItem() const { return m_eItemType == ITEM_TOOL; }
	void SetItemNum(int num) { m_iItemNum = num; }
	int GetItemNum() const { return m_iItemNum; }

	void SetItemSellPrice(int price) { m_iSellPrice = price; }
	int GetItemSellPrice() const { return m_iSellPrice; }
	void SetPrice(int price) { m_iPrice = price; m_iSellPrice = max(price - 20, 0); }
	int GetPrice() const { return m_iPrice; }

	void Increase() { ++m_iItemNum; }
	void Decrease();
	static bool SortByName(const Item* pItemA, const Item* pItemB)
	{
		return pItemA->GetTag() < pItemB->GetTag();
	}
protected:
	Item();
	Item(const Item& item);
	virtual ~Item();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual Item* Clone();
};