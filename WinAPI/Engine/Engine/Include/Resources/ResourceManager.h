#include "../framework.h"
#include "Texture.h"
class ResourceManager {
	DECLARE_SINGLE(ResourceManager)
private:
	unordered_map<string, class Texture*> m_mapTexture;
	static unordered_map<char, class Texture*> m_mapGlyph;
	HINSTANCE m_hInst;
	HDC	      m_hDC;
private:
	class Texture* m_pBackBuffer = nullptr;
	class Texture* m_pTempBuffer = nullptr;
	class Texture* m_pEmptyBuffer = nullptr;
	BLENDFUNCTION m_tBlenFunc = {};
	BLENDFUNCTION m_tTransparent = {};
	void LoadGlyphs();
	static bool IsExistFont(char c)
	{
		return m_mapGlyph.find(c) != m_mapGlyph.end();
	}
public:
	class Texture* GetBackBuffer() const;
	class Texture* GetEmptyBuffer() const;
	class Texture* GetTempBuffer() const;
	void SetAlphaChannel(int alpha) { m_tBlenFunc.SourceConstantAlpha = alpha; }
	BLENDFUNCTION GetBlendFunc() const { return m_tBlenFunc; }
	const BLENDFUNCTION& GetTransparentFunc() const { return m_tTransparent; }
	void ClearBuffer(Texture* pTex, int px, int py, int w, int h);
	static inline void DrawFontsAt(HDC hdc, const string& str, const Pos& pos, DIRECTION align = RIGHT, int padding = 0)
	{
		return DrawFontsAt(hdc, str, int(pos.x), int(pos.y), align, padding);
	}
	static inline void DrawFontsAt(HDC hdc, const string& str, int px, int py, DIRECTION align = RIGHT, int padding = 0)
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
	static inline void DrawFontsAtFixedSize(HDC hdc, const string& str, const Pos& pos, int size_x, int size_y, DIRECTION align = RIGHT, int padding = 0)
	{
		return DrawFontsAtFixedSize(hdc, str, int(pos.x), int(pos.y), size_x, size_y, align, padding);
	}
	static inline void DrawFontsAtFixedSize(HDC hdc, const string& str, int px, int py, int size_x, int size_y, DIRECTION align = RIGHT, int padding = 0)
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
				px += size_x;
				continue;
			}
			if (IsExistFont(str[i]))
			{
				m_mapGlyph[str[i]]->DrawImageAtFixedSize(hdc, px, py, size_x, size_y);
				px += size_x;
			}
		}
	}
public:
	bool Init(HINSTANCE hInst, HDC hDC);
	// strkey = 텍스쳐 명칭, pFileName = 파일 이름, strPathKey = 텍스쳐 base 경로 
	class Texture* LoadTexture(const string& strKey,
								const wchar_t* pFileName,
								const string& strPathKey = TEXTURE_PATH);
	vector<Texture*> LoadTextureFromDirectory(const wchar_t * folderPath, COLORREF chromaKey = RGB(255, 0, 255), const string& strPathKey = TEXTURE_PATH);
	class Texture* LoadTexture(FILE* pFile);
	class Texture* FindTexture(const string& strKey);
	void AddTexture(const string& strKey, Texture* pTexture);
};