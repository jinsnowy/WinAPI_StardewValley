#pragma once
#include "../Object/Ref.h"
#include "../framework.h"
#include "ResourceManager.h"
#include "../Application/Window.h"

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
    inline void DrawImageFrom(int px, int py, int sx, int sy, Texture* pTex, int u, int v)
    {
        if (pTex->GetColorKeyEnable())
        {
            TransparentBlt(m_hMemDC, px, py, sx, sy,
                pTex->GetDC(), u, v,
                sx, sy,
                pTex->GetColorKey());
        }
        else
        {
            BitBlt(m_hMemDC, px, py, sx, sy, pTex->GetDC(), u, v, SRCCOPY);
        }
    }
    inline void DrawImageFrom(int px, int py, int sx, int sy, HDC orgin_hDC, int u, int v)
    {
        BitBlt(m_hMemDC, px, py, sx, sy, orgin_hDC, u, v, SRCCOPY);
    }
    inline void TransparentEffect(HDC hdc, int px, int py, int sx, int sy, int u, int v, unsigned char alpha = 125)
    {
        // 알파 블렌딩 버그 (클라이언트 영역 벗어나면 드로우 안됌)
        if (py < 0)
        {
            py = 0;
            sy += py;
        }
        else if (py + sy >= GETRESOLUTION.y)
        {
            sy -= (py + sy - GETRESOLUTION.y + 1);
        }
        if (px < 0)
        {
            px = 0;
            sx += px;
        }
        else if (px + sx >= GETRESOLUTION.x)
        {
            sx -= (px + sx - GETRESOLUTION.x + 1);
        }
        Texture* pBack = RESOURCE_MANAGER->GetBackBuffer();
        Texture* pTemp = RESOURCE_MANAGER->GetTempBuffer();
        pTemp->ClearBuffer(px, py, sx, sy);
        pTemp->DrawImageFrom(px, py, sx, sy, pBack, px, py);
        pTemp->DrawImageFrom(px, py, sx, sy, this, u, v);

        RESOURCE_MANAGER->SetAlphaChannel(alpha);
        AlphaBlend(hdc, px, py, sx, sy, pTemp->GetDC(), px, py, sx, sy, RESOURCE_MANAGER->GetBlendFunc());
        SAFE_RELEASE(pTemp);
        SAFE_RELEASE(pBack);
    }
    inline void DrawImage(HDC hdc, int px, int py, int sx, int sy, int u, int v)
    {
        if (GetColorKeyEnable())
        {
            TransparentBlt(hdc, px, py, sx, sy,
                m_hMemDC, u, v,
                sx, sy,
                m_ColorKey);
        }
        else
        {
            BitBlt(hdc, px, py, sx, sy, m_hMemDC, u, v, SRCCOPY);
        }
    }
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
    inline void DrawImageAtFixedSize(HDC hdc, int px, int py, int size_x, int size_y, bool keep = false)
    {
        auto tSize = (Vec2I) GetSize();
        if (keep)
        {
            if (tSize.x < size_x)
            {
                px += (size_x - tSize.x) / 2;
                size_x = tSize.x;
            }
            if (tSize.y < size_y)
            {
                py += (size_y - tSize.y) / 2;
                size_y = tSize.y;
            }
        }

        if (GetColorKeyEnable())
        {
            Texture* pEmpty = RESOURCE_MANAGER->GetEmptyBuffer();
            pEmpty->ClearBuffer(0, 0, size_x, size_y);
            StretchBlt(pEmpty->GetDC(), 0, 0, size_x, size_y, GetDC(), 0, 0, int(tSize.x), int(tSize.y), SRCCOPY);
            TransparentBlt(hdc, px, py, size_x, size_y,
                pEmpty->GetDC(), 0, 0,
                size_x, size_y,
                GetColorKey());
            SAFE_RELEASE(pEmpty);
        }
        else
        {
            StretchBlt(hdc, px, py, size_x, size_y, GetDC(), 0, 0, int(tSize.x), int(tSize.y), SRCCOPY);
        }
    }
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