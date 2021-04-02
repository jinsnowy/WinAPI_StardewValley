#include "UITileSelect.h"
#include "Tile.h"
#include "UIButton.h"
#include "../../Application/Window.h"
#include "../../Core/PathManager.h"
#include "../../Resources/Texture.h"
#include "../../Resources/ResourceManager.h"
#include "../../Resources/PrototypeManager.h"
#include "../../Object/Object.h"
#include "../../Core/Input.h"
#include "../../Scene/Scene.h"
#include <cassert>

UITileSelect::UITileSelect()
{
}

UITileSelect::~UITileSelect()
{
    for (auto& kv : m_BaseTileMap)
    {
        Safe_Release_VecList(kv.second);
    }
    m_BaseTileMap.clear();
  
    Safe_Release_VecList(m_PrototypeContainer);
    Safe_Release_VecList(m_NumberTiles);
    Safe_Release_VecList(m_btn);

    INPUT->DeleteKey("NextObject");
    INPUT->DeleteKey("PreviousObject");
}

bool UITileSelect::Init()
{
    INPUT->AddKey("NextObject", 'E');
    INPUT->AddKey("PreviousObject", 'Q');

    SetTexture("Ground", m_strUIImagePath);
    SetAsTextureSize();
    SetPos(GETRESOLUTION.x - m_pTexture->GetWidth() - 50, 200.f);

    Size tSize = GetSize();
    m_iValidWidth = int(tSize.x) - 2 * m_iMarginWidth + m_iMarginItem;
    m_iValidHeight = int(tSize.y) - 2 * m_iMarginHeight + m_iMarginItem;
    m_iDrawMaxitemNumX = m_iValidWidth / (TILESIZE + m_iMarginItem);
    m_iDrawMaxitemNumY = m_iValidHeight / (TILESIZE + m_iMarginItem);
    m_iTotalSizeX = m_iDrawMaxitemNumX * TILESIZE + (m_iDrawMaxitemNumX - 1) * m_iMarginItem;
    m_iTotalSizeY = m_iDrawMaxitemNumY * TILESIZE + (m_iDrawMaxitemNumY - 1) * m_iMarginItem;

    const wchar_t* pPath = PATH_MANAGER->FindPath(TEXTURE_PATH);
    wstring strPath;
    if (pPath)
        strPath = pPath;

    strPath += m_strNumberPath;
    assert(strPath.back() == L'\\' || strPath.back() == L'/');

    for (const auto& entry : fs::directory_iterator(strPath))
    {
        const wchar_t* imgPath = entry.path().c_str();
        string strTexkey = ExtractKey(GetChar(imgPath), lstrlen(imgPath));
        Tile* pTile = Object::CreateObject<Tile>("Tile");

        wstring pFileName = m_strNumberPath;
        wstring strwTexKey(strTexkey.begin(), strTexkey.end());
        pFileName += strwTexKey;
        pFileName += L".bmp";

        pTile->SetTexture(strTexkey, pFileName.c_str());
        pTile->SetColorKey(255, 255, 255);
        pTile->SetAsTextureSize();

        m_NumberTiles.push_back(pTile);
    }

    m_optNameMap.insert(make_pair("1.NoOption", TO_NONE));
    m_optNameMap.insert(make_pair("2.NoMove", TO_NOMOVE));
    m_optNameMap.insert(make_pair("3.CropGround", TO_CROP_GROUND));
    m_optNameMap.insert(make_pair("4.Beacon1", TO_BEACON_1));
    m_optNameMap.insert(make_pair("4.Beacon2", TO_BEACON_2));
    m_optNameMap.insert(make_pair("4.Beacon3", TO_BEACON_3));
    return true;
}


bool UITileSelect::IsOpt(const string& optName) const
{
    return m_optNameMap.find(optName) != m_optNameMap.end();
}

TILE_OPTION UITileSelect::GetOpt(const string& optName) const
{
    auto found = m_optNameMap.find(optName);
    if (found == m_optNameMap.end())
        return TO_NONE;
    return static_cast<TILE_OPTION>(found->second);
}

Object* UITileSelect::GetClickObject(const Pos& screenPos, bool &UITouch)
{
    UITouch = UITouched(screenPos);
    Pos tPos = GetPos();
    Pos tSize = GetSize();

    // 페이지 클릭
    int itemNum = m_eCurSelTile == SEL_OBJECT ?
        (int)m_PrototypeContainer.size()
        : (int)m_BaseTileMap[m_eCurSelTile].size();

    int px = tPos.x, py = tPos.y;
    int pageNum = itemNum / (m_iDrawMaxitemNumY * m_iDrawMaxitemNumX) + 1;
    for (int i = 0; i < pageNum; i++)
    {
        if (screenPos.x >= px - m_iSelButtonSize && screenPos.x < px
            && screenPos.y >= py && screenPos.y < py + m_iSelButtonSize)
        {
            m_iCurPageNum = i;
            return nullptr;
        }
        py += m_iSelButtonSize;
    }

    return m_eCurSelTile == SEL_OBJECT ?
        SelectObject(screenPos) : SelectTile(screenPos);
}

Object* UITileSelect::SelectTile(const Pos& screenPos)
{
    Pos tPos = GetPos();
    Pos tSize = GetSize();

    int st_x = tPos.x + (tSize.x - m_iTotalSizeX) / 2;
    int st_y = tPos.y + (tSize.y - m_iTotalSizeY) / 2;
    int px = st_x, py = st_y, itemInd = m_iCurPageNum * m_iDrawMaxitemNumX * m_iDrawMaxitemNumY;
    int size = (int) m_BaseTileMap[m_eCurSelTile].size();
    for (int j = 0; j < m_iDrawMaxitemNumY && itemInd < size; ++j)
    {
        for (int i = 0; i < m_iDrawMaxitemNumX && itemInd < size; ++i)
        {
            if (screenPos.x >= px && screenPos.x < px + TILESIZE
                && screenPos.y >= py && screenPos.y < py + TILESIZE)
            {
                m_BaseTileMap[m_eCurSelTile][itemInd]->AddRef();
                return m_BaseTileMap[m_eCurSelTile][itemInd];
            }
            ++itemInd;
            px += (TILESIZE + m_iMarginItem);
        }
        px = st_x;
        py += (TILESIZE + m_iMarginItem);
    }
    return nullptr;
}

Object* UITileSelect::SelectObject(const Pos& screenPos)
{
    vector<Object*>::const_iterator iter = m_PrototypeContainer.begin();
    vector<Object*>::const_iterator iterEnd = m_PrototypeContainer.end();

    Pos tPos = GetPos();
    Pos tSize = GetSize();
    int st_x = tPos.x + (tSize.x - m_iTotalSizeX) / 2;
    int st_y = tPos.y + (tSize.y - m_iTotalSizeY) / 2;

    int px = st_x, py = st_y;
    int itemInd = m_iCurPageNum * m_iDrawMaxitemNumX * m_iDrawMaxitemNumY;
    for (int j = 0; j < m_iDrawMaxitemNumY && iter != iterEnd; ++j)
    {
        for (int i = 0; i < m_iDrawMaxitemNumX && iter != iterEnd; ++i)
        {
            if (screenPos.x >= px && screenPos.x < px + TILESIZE
                && screenPos.y >= py && screenPos.y < py + TILESIZE)
            {
                return (*iter)->Clone();
            }
            ++iter;
            px += (TILESIZE + m_iMarginItem);
        }
        px = st_x;
        py += (TILESIZE + m_iMarginItem);
    }
    return nullptr;
}

void UITileSelect::LoadTiles(EDIT_MODE eSel, const wchar_t* pBaseFolderName, const string& strPathKey)
{
    const wchar_t* pPath = PATH_MANAGER->FindPath(strPathKey);
    wstring strPath;
    if (pPath)
        strPath = pPath;

    strPath += pBaseFolderName;
    assert(strPath.back() == L'\\' || strPath.back() == L'/');
    std::error_code ec; 
    for (const auto& entry : fs::directory_iterator(strPath))
    {
        const wchar_t * imgPath = entry.path().c_str();
        if (fs::is_directory(imgPath, ec))
        {
            continue;
        }
        string strTexkey = ExtractKey(GetChar(imgPath), lstrlen(imgPath));
        Tile* pTile = Object::CreateObject<Tile>("Tile");

        wstring pFileName = pBaseFolderName;
        wstring strwTexKey(strTexkey.begin(), strTexkey.end());
        pFileName += strwTexKey;
        pFileName += L".bmp";
   
        pTile->SetTexture(strTexkey, pFileName.c_str());
        pTile->SetColorKey(255, 255, 255);
        pTile->SetPivot(0.f, 1.0f);
        pTile->SetAsTextureSize();

        m_BaseTileMap[int(eSel)].push_back(pTile);
    }
}

void UITileSelect::LoadPrototypes(PR_TYPE eType)
{
    unordered_map<string, Object*>* prototypes = PROTOTYPE_MANAGER->GetPrototypes(eType);
    for (auto iter = prototypes->begin(); iter!= prototypes->end(); ++iter)
    {
        m_PrototypeContainer.push_back(iter->second);
        iter->second->AddRef();
    }

    const auto sort_by = [](Object* pa, Object* pb)
    {
        return pa->GetTexTag() < pb->GetTexTag();
    };
    sort(m_PrototypeContainer.begin(), m_PrototypeContainer.end(), sort_by);
}

void UITileSelect::ChangeState()
{
    m_btn[m_eCurSelTile]->SetImageOffset(0, 0);
    m_eCurSelTile = static_cast<EDIT_MODE>((m_eCurSelTile + 1) % int(SEL_END));
    m_btn[m_eCurSelTile]->SetImageOffset(120, 0);
    m_iCurPageNum = 0;
}

void UITileSelect::Input(float dt)
{
    UI::Input(dt);
    int itemNum = m_eCurSelTile == SEL_OBJECT ?
        (int) m_PrototypeContainer.size()
        : (int) m_BaseTileMap[m_eCurSelTile].size();

    int pageNum = itemNum / (m_iDrawMaxitemNumY * m_iDrawMaxitemNumX) + 1;
    if (KEYDOWN("NextObject"))
    {
        m_iCurPageNum = (m_iCurPageNum + 1) % pageNum;
    }
    if (KEYDOWN("PreviousObject"))
    {
        m_iCurPageNum = (m_iCurPageNum + pageNum - 1) % pageNum;
    }
    if (UITouched(MOUSECLIENTPOS) && KEYDOWN("MouseLButton"))
    {
        m_ClickPos = MOUSECLIENTPOS - GetPos();
        if (!m_bDrag)
        {
            m_bDrag = true;
        }
    }
    if (KEYPRESS("MouseLButton"))
    {
        if (m_bDrag)
        {
            SetPos(MOUSECLIENTPOS - m_ClickPos);
        }
    }
    if (KEYUP("MouseLButton"))
    {
        if (m_bDrag)
        {
            m_bDrag = false;
        }
    }
}

int UITileSelect::Update(float dt)
{
    UI::Update(dt);
    return 0;
}

int UITileSelect::LateUpdate(float dt)
{
    UI::LateUpdate(dt);
    return 0;
}

void UITileSelect::Collision(float dt)
{
    UI::Collision(dt);
}

void UITileSelect::Draw(HDC hdc, float dt)
{
    UI::Draw(hdc, dt);
    m_eCurSelTile == SEL_OBJECT ? DrawObjectPanel(hdc, dt) : DrawTilePanel(hdc, dt);
    DrawPageNumPanel(hdc, dt);
}

bool UITileSelect::UITouched(const Pos& screenPos) const
{
    Pos tPos = GetPos();
    Pos tSize = GetSize();
  
    if (screenPos.x >= tPos.x && screenPos.x < tPos.x + tSize.x
        && screenPos.y >= tPos.y && screenPos.y < tPos.y + tSize.y)
    {
        return true;
    }
    return false;
}

void UITileSelect::SetUpTagButton(Scene* curScene)
{
    Layer* pLayer = curScene->FindLayer("UI");

    m_btn.resize(m_btnFileName.size(), nullptr);
    wstringstream path;
    for (int i = 0; i < m_btnFileName.size(); ++i)
    {
        path << L"SV/Numbers/Tag/" << m_btnFileName[i] << L".bmp";
        string strKey(GetChar(m_btnFileName[i].c_str()));
        m_btn[i] = curScene->CreateObject<UIButton>(strKey, pLayer);
        m_btn[i]->SetTexture(strKey, path.str().c_str());
        m_btn[i]->SetSize(120, 60);
        m_btn[i]->SetColorKey(255, 255, 255);
        if (i == 0)
        {
            m_btn[i]->SetImageOffset(120, 0);
        }
        Size tSize = m_btn[i]->GetSize();
        m_btn[i]->SetPos(20 + i * tSize.x, 20);
        path.clear();
        path.str(L"");
    }
}

string UITileSelect::ExtractKey(const char* str, int size)
{
    int ed = size - 1;
    while (str[ed] != L'.') --ed;
    int st = ed - 1;
    while (str[st] != L'\\' && str[st] != L'/') st--;
    return string(str + st + 1, str + ed);
}

void UITileSelect::DrawTilePanel(HDC hdc, float dt)
{
    // 타일 번호
    Pos tPos = GetPos();
    Pos tSize = GetSize();
    int st_x = tPos.x + (tSize.x - m_iTotalSizeX) / 2;
    int st_y = tPos.y + (tSize.y - m_iTotalSizeY) / 2;
    int tileNum = (int) m_BaseTileMap[int(m_eCurSelTile)].size();

    int px = st_x, py = st_y;
    int itemInd = m_iCurPageNum * m_iDrawMaxitemNumX * m_iDrawMaxitemNumY;
    for (int j = 0; j < m_iDrawMaxitemNumY && itemInd < tileNum; ++j)
    {
        for (int i = 0; i < m_iDrawMaxitemNumX && itemInd < tileNum; ++i)
        {
            m_BaseTileMap[int(m_eCurSelTile)][itemInd++]->DrawImageAtFixedSize(hdc, px, py, TILESIZE, true);
            px += (TILESIZE + m_iMarginItem);
        }
        px = st_x;
        py += (TILESIZE + m_iMarginItem);
    }
}

void UITileSelect::DrawObjectPanel(HDC hdc, float dt)
{
    vector<Object*>::const_iterator iter = m_PrototypeContainer.begin();
    vector<Object*>::const_iterator iterEnd = m_PrototypeContainer.end();

    Pos tPos = GetPos();
    Pos tSize = GetSize();
    int st_x = tPos.x + (tSize.x - m_iTotalSizeX) / 2;
    int st_y = tPos.y + (tSize.y - m_iTotalSizeY) / 2;

    int px = st_x, py = st_y;
    int itemInd = m_iCurPageNum * m_iDrawMaxitemNumX * m_iDrawMaxitemNumY;
    for (int j = 0; j < m_iDrawMaxitemNumY && iter != iterEnd; ++j)
    {
        for (int i = 0; i < m_iDrawMaxitemNumX && iter != iterEnd; ++i)
        {
            (*iter)->DrawImageAtFixedSize(hdc, px, py, TILESIZE, true);
            ++iter;
            px += (TILESIZE + m_iMarginItem);
        }
        px = st_x;
        py += (TILESIZE + m_iMarginItem);
    }
}

void UITileSelect::DrawPageNumPanel(HDC hdc, float dt)
{
    Pos tPos = GetPos();

    int itemNum = m_eCurSelTile == SEL_OBJECT ?
        (int)m_PrototypeContainer.size()
        : (int)m_BaseTileMap[m_eCurSelTile].size();

    int px = int(tPos.x), py = int(tPos.y);
    int pageNum = itemNum / (m_iDrawMaxitemNumY * m_iDrawMaxitemNumX) + 1;
    for (int i = 0; i < pageNum; i++)
    {
        m_NumberTiles[i%10]->DrawImageAtFixedSize(hdc, px- m_iSelButtonSize, py, m_iSelButtonSize);
        if (i == m_iCurPageNum)
        {
            DrawRedRect(hdc, MakeRect(px - m_iSelButtonSize, py, m_iSelButtonSize, m_iSelButtonSize));
        }
        py += m_iSelButtonSize;
    }
}
