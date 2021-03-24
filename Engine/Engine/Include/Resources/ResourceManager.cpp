#include "ResourceManager.h"
#include "Texture.h"
#include "../Object/Object.h"
#include "../Application/Window.h"
#include "../Core/PathManager.h"

DEFINITION_SINGLE(ResourceManager)
unordered_map<char, class Texture*> ResourceManager::m_mapGlyph;

ResourceManager::ResourceManager()
	: m_hInst(nullptr),
	  m_hDC(nullptr)
{
}

ResourceManager::~ResourceManager()
{
	SAFE_RELEASE(m_pBackBuffer);
	SAFE_RELEASE(m_pTempBuffer);
	SAFE_RELEASE(m_pEmptyBuffer);
	Safe_Release_Map(m_mapTexture);
	Safe_Release_Map(m_mapGlyph);
}

void ResourceManager::LoadGlyphs()
{
	vector<Texture*> fonts = LoadTextureFromDirectory(L"SV/Fonts/", RGB(255, 255, 255));
	for (Texture* pTex : fonts)
	{
		if (pTex)
		{
			string strKey = pTex->GetTexTag();
			char ch = (char)stoi(strKey);
			pTex->AddRef();
			m_mapGlyph.insert(make_pair(ch, pTex));
		}
	}
	Safe_Release_VecList(fonts);

	Texture* pFontSheet = LoadTexture("FontSheet", L"SV/Fonts.bmp");
	
	wstring strPath = PATH_MANAGER->FindPath(TEXTURE_PATH);
	strPath += L"SV/FontOffset.csv";
	
	ifstream file(GetChar(strPath.c_str()));

	if (file.fail())
	{
		throw EXCEPT(L"Font Offset file not found");
	}

	string line;
	getline(file, line);

	while (file.good())
	{
		getline(file, line);
	
		vector<string> elements;
		size_t prev = 0, cur = line.find(',');
		while(true)
		{
			elements.emplace_back(line.substr(prev, cur - prev));

			if (cur == string::npos)
				break;

			prev = cur + 1;
			cur = line.find(',', prev);
		} 

		if (elements.size() == 5)
		{
			char ch = elements[0][0];
			int offsetX = stoi(elements[1]), offsetY = stoi(elements[2]);
			int width = stoi(elements[3]), height = stoi(elements[4]);

			Texture* pFontTex = Texture::CreateEmptyTexture(m_hDC, width, height);
			pFontTex->DrawImageFrom(0, 0, width, height, pFontSheet->GetDC(), offsetX, offsetY);
			pFontTex->SetColorKey(255, 255, 255);
			pFontTex->SetWidth((long)width);
			pFontTex->SetHeight((long)height);
			m_mapGlyph.insert(make_pair(ch, pFontTex));
		}
	}

	SAFE_RELEASE(pFontSheet);
}


Texture* ResourceManager::GetBackBuffer() const
{
	m_pBackBuffer->AddRef();
	return m_pBackBuffer;
}

Texture* ResourceManager::GetEmptyBuffer() const
{
	m_pEmptyBuffer->AddRef();
	return m_pEmptyBuffer;
}

Texture* ResourceManager::GetTempBuffer() const
{
	m_pTempBuffer->AddRef();
	return m_pTempBuffer;
}

void ResourceManager::ClearBuffer(Texture* pTex, int px, int py, int w, int h)
{
	pTex->ClearBuffer(px, py, w, h);
}

void ResourceManager::DrawFontsAt(HDC hdc, const string& str, int px, int py, DIRECTION align, int padding)
{
	int length = (int)str.size();

	int totalWidth = 0;
	int maxWidth = 0;
	int maxHeight = 0;
	for (int i = 0; i < length; ++i)
	{
		if (IsExistFont(str[i]))
		{
			Texture* pFont = m_mapGlyph[str[i]];
			totalWidth += (int)pFont->GetWidth();
			maxHeight = max(maxHeight, (int)pFont->GetHeight());
			maxWidth = max(maxWidth, (int)pFont->GetWidth());
		}
	}

	if (padding > 0) {
		assert(padding >= length);
		switch (align)
		{
		case RIGHT:
			px += (padding * maxWidth - totalWidth);
			break;
		case CENTER:
			px += ((padding * maxWidth - totalWidth)) / 2;
			if ((padding - length) % 2)
				px += maxWidth / 2;
			break;
		}
	}

	for (int i = 0; i < length; ++i)
	{
		if (str[i] == ' ' || str[i] == '_')
		{
			px += maxWidth;
			continue;
		}
		if (IsExistFont(str[i]))
		{
			Texture* pFont = m_mapGlyph[str[i]];
			int marginY = maxHeight - (int)pFont->GetHeight();
			pFont->DrawImageAt(hdc, px, py + marginY);
			px += pFont->GetWidth();
		}
	}
}
void ResourceManager::DrawFontsAtFixedSize(HDC hdc, const string& str, int px, int py, int size_x, int size_y, bool keep, DIRECTION align, int padding)
{
	int totalWidth = 0;
	int length = (int)str.size();
	for (int i = 0; i < length; ++i)
	{
		if (IsExistFont(str[i]))
		{
			Texture* pFont = m_mapGlyph[str[i]];
			totalWidth += size_x;
		}
	}

	if (padding > 0) {
		assert(padding >= length);
		switch (align)
		{
		case RIGHT:
			px += (padding * size_x - totalWidth);
			break;
		case CENTER:
			px += ((padding * size_x - totalWidth)) / 2;
			if ((padding - length) % 2)
				px += size_x / 2;
			break;
		}
	}

	for (int i = 0; i < length; ++i)
	{
		if (str[i] == ' ' || str[i] == '_')
		{
			px += size_x;
			continue;
		}
		if (IsExistFont(str[i]))
		{
			m_mapGlyph[str[i]]->DrawImageAtFixedSize(hdc, px, py, size_x, size_y, keep);
			px += size_x;
		}
	}
}

bool ResourceManager::Init(HINSTANCE hInst, HDC hDC)
{
	m_hInst = hInst;
	m_hDC = hDC;

	LoadGlyphs();
	// 백버퍼를 불러온다.
	m_pBackBuffer = LoadTexture("BackBuffer", L"BackBuffer.bmp");

	// 빈버퍼
	m_pEmptyBuffer = Texture::CreateEmptyTexture(hDC, GETRESOLUTION.x, GETRESOLUTION.y);

	m_pTempBuffer = Texture::CreateEmptyTexture(hDC, GETRESOLUTION.x, GETRESOLUTION.y);

	m_tBlenFunc.BlendOp = AC_SRC_OVER;
	m_tBlenFunc.BlendFlags = 0;
	m_tBlenFunc.SourceConstantAlpha = 0;
	m_tBlenFunc.AlphaFormat = 0;

	m_tTransparent.BlendOp = AC_SRC_OVER;
	m_tTransparent.BlendFlags = 0;
	m_tTransparent.SourceConstantAlpha = 100;
	m_tTransparent.AlphaFormat = 0;
	return true;
}

Texture* ResourceManager::LoadTexture(const string& strKey,
									  const wchar_t* pFileName,
									  const string& strPathKey)
{
	assert(strKey.size());
	// 먼저 그전에 등록된 텍스쳐인지 확인한다.
	Texture* pTexture = FindTexture(strKey);
	if (pTexture)
		return pTexture;

	pTexture = new Texture;

	if (!pTexture->LoadTexture(m_hInst, m_hDC, strKey, pFileName, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return nullptr;
	}

	// 새로 텍스쳐를 등록 
	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

vector<Texture*> ResourceManager::LoadTextureFromDirectory(const wchar_t* folderPath, COLORREF chromaKey, const string& strPathKey)
{
	vector<Texture*> vecTex;
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
		string strTexkey = ExtractKeyFromPathString(GetChar(imgPath), lstrlen(imgPath));
		Texture* pTexture = LoadTexture(strTexkey, imgPath, "");
		pTexture->SetColorKey(chromaKey);
		vecTex.push_back(pTexture);
	}
	return vecTex;
}

Texture* ResourceManager::LoadTexture(FILE* pFile)
{
	int iLength = 0;

	char strKey[MAX_PATH] = {};
	char strPathKey[MAX_PATH] = {};
	wchar_t strFileName[MAX_PATH] = {};

	fread(&iLength, 4, 1, pFile);
	fread(strKey, 1, iLength, pFile);
	strKey[iLength] = 0;


	iLength = 0;
	fread(&iLength, 4, 1, pFile);
	fread(strFileName, 2, iLength, pFile);
	strFileName[iLength] = 0;

	// PathKey 저장
	iLength = 0;
	fread(&iLength, 4, 1, pFile);
	fread(strPathKey, 1, iLength, pFile);
	strPathKey[iLength] = 0;

	Texture* pTexture = RESOURCE_MANAGER->LoadTexture(strKey, strFileName, strPathKey);

	// ColorKey
	if (pTexture)
	{
		fread(&pTexture->m_bColorKeyEnable, 1, 1, pFile);
		fread(&pTexture->m_ColorKey, sizeof(COLORREF), 1, pFile);
	}

	return pTexture;
}

Texture* ResourceManager::FindTexture(const string& strKey)
{
	auto iter = m_mapTexture.find(strKey);
	if (iter == m_mapTexture.end())
		return nullptr;

	iter->second->AddRef();
	return iter->second;
}

void ResourceManager::AddTexture(const string& strKey, Texture* pTexture)
{
	if (FindTexture(strKey))
		return;
	
	if (pTexture)
	{
		pTexture->AddRef();
		m_mapTexture.insert(make_pair(strKey, pTexture));
	}
}

