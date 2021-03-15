#pragma once
#include "UI.h"
#include "../../framework.h"
class UITileSelect :
    public UI
{
private:
	friend class Layer;
	friend class Scene;
	friend class MapEditScene;
	const vector<wstring> m_btnFileName = { L"1.GroundSelTag",
										L"2.StaticSelTag",
										L"3.TileSelTag",
										L"4.ObjectSelTag",
										L"5.OptionSelTag", };
	vector<class UIButton*> m_btn;
	const wchar_t *m_strNumberPath = L"SV/Numbers/Select/";
	const wchar_t* m_strUIImagePath = L"SV/UISelectBase2.bmp";
	unordered_map<string, int> m_optNameMap;

	int m_iDrawMaxitemNumX = 0;
	int m_iDrawMaxitemNumY = 0;
	int m_iValidWidth = 0;
	int m_iValidHeight = 0;
	int m_iTotalSizeX = 0;
	int m_iTotalSizeY = 0;
private:
	bool m_bDrag = false;
	Pos m_ClickPos;
	int m_iCurPageNum = 0;
	EDIT_MODE m_eCurSelTile = SEL_GROUND;

	int m_iMarginWidth = 10;
	int m_iMarginHeight = 10;
	int m_iMarginItem = 10;
	static constexpr int m_iSelButtonSize = 40;
	static constexpr int m_iStageTagWidth = 120;
	static constexpr int m_iStageTagHeight = 80;
	unordered_map<int, vector<class Tile*>> m_BaseTileMap;
	vector<class Tile*> m_NumberTiles;
	vector<Object*> m_PrototypeContainer;
public:
	bool IsOpt(const string& optName) const;
	TILE_OPTION GetOpt(const string& optName) const;
	EDIT_MODE GetCurSelect() const { return m_eCurSelTile; }
	Object* GetClickObject(const Pos& screenPos, bool &UITouch);
	void LoadTiles(EDIT_MODE eSel, const wchar_t* pBaseFolderName, const string& strPathKey = TEXTURE_PATH);
	void LoadPrototypes(PR_TYPE eType);
	void ChangeState();
protected:
	UITileSelect();
	UITileSelect(const UITileSelect& ui) = delete;
	virtual ~UITileSelect();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual UITileSelect* Clone() { throw EXCEPT(L"UITileSelct Cloned"); return nullptr; }
private:
	bool UITouched(const Pos& screenPos) const;
	void SetUpTagButton(class Scene* curScene);
	string ExtractKey(const char* str, int size);
	Object* SelectObject(const Pos& screenPos);
	Object* SelectTile(const Pos& screenPos);
	void DrawTilePanel(HDC hdc, float dt);
	void DrawObjectPanel(HDC hdc, float dt);
	void DrawPageNumPanel(HDC hdc, float dt);
};

