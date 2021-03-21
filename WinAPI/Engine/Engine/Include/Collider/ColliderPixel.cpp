#include "ColliderPixel.h"
#include "ColliderRect.h"
#include "ColliderSphere.h"
#include "ColliderPoint.h"
#include "../Object/Object.h"
#include "../Core/PathManager.h"

ColliderPixel::ColliderPixel()
{
    m_eCollType = CT_PIXEL;
    m_iWidth = 0;
    m_iHeight = 0;
}

ColliderPixel::ColliderPixel(const ColliderPixel& coll)
    :
    Collider(coll)
{
    m_iWidth = coll.m_iWidth;
    m_iHeight = coll.m_iHeight;
}

ColliderPixel::~ColliderPixel()
{
}

bool ColliderPixel::SetPixelInfo(const char* pFileName, const string& strPathKey)
{
    m_strFileName = pFileName;
    m_strPathKey = strPathKey;

    const char* pPath = PATH_MANAGER->FindPathByMultiByte(strPathKey);
    string strPath;

    if (pPath)
        strPath = pPath;

    strPath += pFileName;

    FILE* pFile = NULL;

    fopen_s(&pFile, strPath.c_str(), "rb");

    if (!pFile)
        return false;

    BITMAPFILEHEADER fh;
    BITMAPINFOHEADER ih;

    fread(&fh, sizeof(fh), 1, pFile);
    fread(&ih, sizeof(ih), 1, pFile);

    m_iWidth = ih.biWidth;
    m_iHeight = ih.biHeight;

    m_vecPixel.clear();

    int totalSize = m_iWidth * m_iHeight;
    m_vecPixel.resize(totalSize);

    fread(&m_vecPixel[0], sizeof(Pixel), m_vecPixel.size(), pFile);

    // 위 아래를 반전시켜준다.
    Pixel* pPixelArr = new Pixel[m_iWidth];
    for (int i = 0; i < m_iHeight / 2; ++i)
    {
        // 현재 인덱스의 픽셀 한 줄을 저장해둔다.
        int low = i * m_iWidth;
        int high = (m_iHeight - 1 - i) * m_iWidth;
        memcpy(pPixelArr, &m_vecPixel[low], sizeof(Pixel) * m_iWidth);
        memcpy(&m_vecPixel[low], &m_vecPixel[high], sizeof(Pixel) * m_iWidth);
        memcpy(&m_vecPixel[high], pPixelArr, sizeof(Pixel) * m_iWidth);
    }

    delete[] pPixelArr;

    fclose(pFile);

    /*fopen_s(&pFile, "Test.bmp", "wb");

    fwrite(&fh, sizeof(fh), 1, pFile);
    fwrite(&ih, sizeof(ih), 1, pFile);
    fwrite(&m_vecPixel[0], sizeof(Pixel), m_vecPixel.size(), pFile);

    fclose(pFile);*/

    return true;
}

Rect ColliderPixel::GetBounds() const
{
    Pos tPos = m_pObject->GetPos();
    return Rect(tPos.x, tPos.y, tPos.x + m_iWidth, tPos.y + m_iHeight);
}

bool ColliderPixel::Init()
{
    return true;
}

void ColliderPixel::Input(float dt)
{
    Collider::Input(dt);
}

int ColliderPixel::Update(float dt)
{
    Collider::Update(dt);
    return 0;
}

int ColliderPixel::LateUpdate(float dt)
{
    Collider::LateUpdate(dt);

    return 0;
}

bool ColliderPixel::CheckCollision(Collider* pDst)
{
    switch (pDst->GetColliderType())
    {
    case CT_RECT:
        return CollisionRectToPixel(static_cast<ColliderRect*>(pDst)->GetWorldInfo(), m_vecPixel, m_iWidth, m_iHeight);
    case CT_POINT:
        return CollisionPixelToPoint(m_vecPixel, m_iWidth, m_iHeight, static_cast<ColliderPoint*>(pDst)->GetWorldInfo());
    case CT_SPHERE:
        return CollisionSphereToPixel(static_cast<ColliderSphere*>(pDst)->GetWorldInfo(), m_vecPixel, m_iWidth, m_iHeight);
    }
    return true;
}

void ColliderPixel::Draw(HDC hdc, float dt)
{
    Collider::Draw(hdc, dt);
}

ColliderPixel* ColliderPixel::Clone()
{
    return new ColliderPixel(*this);
}

void ColliderPixel::Save(FILE* pFile)
{
    Collider::Save(pFile);

    // FileName 저장
    int iLength = m_strFileName.length();
    fwrite(&iLength, 4, 1, pFile);
    fwrite(m_strFileName.c_str(), 2, iLength, pFile);

    // PathKey 저장
    iLength = m_strPathKey.length();
    fwrite(&iLength, 4, 1, pFile);
    fwrite(m_strPathKey.c_str(), 1, iLength, pFile);
}

void ColliderPixel::Load(FILE* pFile)
{
    Collider::Load(pFile);

    int iLength = 0;
    char strFileName[MAX_PATH] = {};
    char strPathKey[MAX_PATH] = {};

    // FileName 저장
    iLength = 0;
    fread(&iLength, 4, 1, pFile);
    fread(strFileName, 1, iLength, pFile);
    strFileName[iLength] = 0;

    // PathKey 저장
    iLength = 0;
    fread(&iLength, 4, 1, pFile);
    fread(strPathKey, 1, iLength, pFile);
    strPathKey[iLength] = 0;

    SetPixelInfo(strFileName, strPathKey);
}
