#pragma once
#include "../Object/Ref.h"
#include "../framework.h"
class Texture :
    public Ref
{
private:
    friend class ResourceManager;
    Texture();
    ~Texture();
public:
    static bool TexSort(const Texture* lhs, const Texture* rhs)
    {
        return lhs->GetTexTag() < rhs->GetTexTag();
    }
    bool LoadTexture(HINSTANCE hInst,
                     HDC hDC,
                     const string& strKey,
                     const wchar_t* pFileName,
                     const string& strPathKey = TEXTURE_PATH);

    void ClearBuffer(int px, int py, int w, int h)
    {
        DrawHDCWithColor(GetDC(), px, py, w, h, RGB(0, 0, 0));
    }
    void DrawImageFrom(int px, int py, int sx, int sy, Texture* pTex, int u, int v);
    void DrawImageFrom(int px, int py, int sx, int sy, HDC orgin_hDC, int u, int v);

    void TransparentEffect(HDC hdc, int px, int py, int sx, int sy, int u, int v, unsigned char alpha = 125);
    void DrawImage(HDC hdc, int px, int py, int sx, int sy, int u, int v);
    
    inline void DrawImageAt(HDC hdc, const Pos& at)
    {
        DrawImageAt(hdc, int(at.x), int(at.y));
    }
    inline void DrawImageAt(HDC hdc, int px, int py)
    {
        Size tSize = GetSize();
        if (GetColorKeyEnable())
        {
            TransparentBlt(hdc, px, py, int(tSize.x), int(tSize.y),
                GetDC(), 0, 0,
                int(tSize.x), int(tSize.y),
                GetColorKey());
        }
        else
        {
            BitBlt(hdc, px, py, int(tSize.x), int(tSize.y),
                GetDC(), 0, 0, SRCCOPY);
        }
    }
    inline void DrawImageAtFixedSize(HDC hdc, const Pos& at, int size, bool keep = false)
    {
        DrawImageAtFixedSize(hdc, int(at.x), int(at.y), size, size, keep);
    }
    inline void DrawImageAtFixedSize(HDC hdc, int px, int py, int size, bool keep = false)
    {
        DrawImageAtFixedSize(hdc, px, py, size, size, keep);
    }
    inline void DrawImageAtFixedSize(HDC hdc, const Pos& at, int size_x, int size_y, bool keep = false)
    {
        DrawImageAtFixedSize(hdc, int(at.x), int(at.y), size_x, size_y, keep);
    }
    void DrawImageAtFixedSize(HDC hdc, int px, int py, int size_x, int size_y, bool keep = false);
    HDC GetDC() const { return m_hMemDC; }
    static Texture* CreateEmptyTexture(HDC hDC, int w, int h, COLORREF color = RGB(0,0,0));
    static Texture* CreateCopyTexture(HDC hDC, int w, int h);
private:
    HDC         m_hMemDC = nullptr;
    HBITMAP     m_hBitmap = nullptr;
    HBITMAP     m_hOldBitmap = nullptr;
    BITMAP      m_tInfo = {};
    COLORREF    m_ColorKey;
    bool        m_bColorKeyEnable = false;
    wstring     m_strFileName;
    string      m_strKey;
    string      m_strPathKey;
public:
    void SetTexTag(const string& str)
    {
        m_strKey = str;
    }
    string GetTexTag() const
    {
        return m_strKey;
    }
    void SetColorKey(unsigned char r, unsigned char g, unsigned char b)
    {
        m_bColorKeyEnable = true;
        m_ColorKey = RGB(r, g, b);
    }
    void SetColorKey(const COLORREF& key)
    {
        m_bColorKeyEnable = true;
        m_ColorKey = key;
    }
    COLORREF GetColorKey() const
    {
        return m_ColorKey;
    }
    bool GetColorKeyEnable() const
    {
        return m_bColorKeyEnable;
    }
    Size GetSize() const { return { float(m_tInfo.bmWidth), float(m_tInfo.bmHeight) }; }
    long GetWidth() const
    {
        return m_tInfo.bmWidth;
    }
    long GetHeight() const
    {
        return m_tInfo.bmHeight;
    }
    void SetWidth(long width)
    {
        m_tInfo.bmWidth = width;
    }
    void SetHeight(long height)
    {
        m_tInfo.bmHeight = height;
    }
    void SaveFromPath(const char* pFileName, const string& strPathKey);
    void Save(FILE* pFile);
    void LoadFromPath(const char* pFileName, const string& strPathKey);
};