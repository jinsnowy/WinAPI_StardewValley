#include "Texture.h"
#include "../Core/PathManager.h"
#include "../Application/Window.h"

Texture::Texture()
    :
    m_hMemDC(nullptr),
    m_bColorKeyEnable(false),
    m_ColorKey(RGB(255, 0, 255))
{
}

Texture::~Texture()
{
    SelectObject(m_hMemDC, m_hOldBitmap);
    DeleteObject(m_hBitmap);
    DeleteDC(m_hMemDC);
}

unique_ptr<Texture, Texture::Deleter> Texture::MakeTexture()
{
    return unique_ptr<Texture, Deleter>(nullptr, Deleter());
}

bool Texture::LoadTexture(HINSTANCE hInst, HDC hDC,
                          const string& strKey,
                          const wchar_t* pFileName,
                          const string& strPathKey)
{
    m_strFileName = pFileName;
    m_strKey = strKey;
    m_strPathKey = strPathKey;

    // 메모리 DC를 만들어준다.
    m_hMemDC = CreateCompatibleDC(hDC);

    // 전체 경로를 만들어준다.
    const wchar_t* pPath = PATH_MANAGER->FindPath(strPathKey);

    wstring strPath;
    if (pPath)
        strPath = pPath;

    strPath += pFileName;

    m_hBitmap = (HBITMAP)LoadImage(hInst, strPath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
    GetObject(m_hBitmap, sizeof(m_tInfo), &m_tInfo);

    if (!m_hBitmap)
        return false;

    return true;
}

Texture* Texture::CreateEmptyTexture(HDC hDC, int w, int h, COLORREF color)
{
    Texture* pTexture = new Texture;

    pTexture->m_hMemDC = CreateCompatibleDC(hDC);
    pTexture->m_hBitmap = CreateCompatibleBitmap(hDC, w, h);
    pTexture->m_hOldBitmap = (HBITMAP)SelectObject(pTexture->m_hMemDC, pTexture->m_hBitmap);
    pTexture->m_tInfo.bmWidth = w;
    pTexture->m_tInfo.bmHeight = h;
    DrawHDCWithColor(pTexture->m_hMemDC, w, h, color);

    return pTexture;
}

Texture* Texture::CreateCopyTexture(HDC hDC, int w, int h)
{
    Texture* pTexture = new Texture;

    pTexture->m_hMemDC = CreateCompatibleDC(hDC);
    pTexture->m_hBitmap = CreateCompatibleBitmap(hDC, w, h);
    pTexture->m_hOldBitmap = (HBITMAP)SelectObject(pTexture->m_hMemDC, pTexture->m_hBitmap);

    BitBlt(pTexture->m_hMemDC, 0, 0, w, h, hDC, 0, 0, SRCCOPY);

    return pTexture;
}


void Texture::SaveFromPath(const char* pFileName, const string& strPathKey)
{
}

void Texture::Save(FILE* pFile)
{
    int iLength = (int) m_strKey.length();

    // key의 길이를 저장한다.
    fwrite(&iLength, 4, 1, pFile);
    fwrite(m_strKey.c_str(), 1, iLength, pFile);

    // FileName 저장
    iLength = (int) m_strFileName.length();
    fwrite(&iLength, 4, 1, pFile);
    fwrite(m_strFileName.c_str(), 2, iLength, pFile);

    // PathKey 저장
    iLength = (int) m_strPathKey.length();
    fwrite(&iLength, 4, 1, pFile);
    fwrite(m_strPathKey.c_str(), 1, iLength, pFile);

    // ColorKey
    fwrite(&m_bColorKeyEnable, 1, 1, pFile);
    fwrite(&m_ColorKey, sizeof(COLORREF), 1, pFile);
}

void Texture::LoadFromPath(const char* pFileName, const string& strPathKey)
{
}
