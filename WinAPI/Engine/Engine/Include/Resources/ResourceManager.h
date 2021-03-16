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

public:
	class Texture* GetBackBuffer() const;
	class Texture* GetEmptyBuffer() const;
	class Texture* GetTempBuffer() const;
	void SetAlphaChannel(int alpha) { m_tBlenFunc.SourceConstantAlpha = alpha; }
	BLENDFUNCTION GetBlendFunc() const { return m_tBlenFunc; }
	const BLENDFUNCTION& GetTransparentFunc() const { return m_tTransparent; }
	void ClearBuffer(Texture* pTex, int px, int py, int w, int h);
	static inline void DrawFontsAt(HDC hdc, const string& str, int px, int py, DIRECTION align = RIGHT, int padding = 0)
	{
		int length = (int)str.size();
		if (padding > 0) {
			assert(padding >= length);
			switch (align)
			{
			case RIGHT:
				px += (padding - length) * GLYPHX;
				break;
			case CENTER:
				px += (padding - length) / 2;
				if ((padding - length) % 2)
					px += GLYPHX / 2;
				break;
			}
		}
		for (int i = 0; i < length; ++i)
		{
			m_mapGlyph[str[i]]->DrawImageAt(hdc, px, py);
			px += GLYPHX;
		}
	}
	static inline void DrawFontsAtFixedSize(HDC hdc, const string& str, const Pos& pos, int size_x, int size_y, DIRECTION align = RIGHT, int padding = 0)
	{
		return DrawFontsAtFixedSize(hdc, str, int(pos.x), int(pos.y), size_x, size_y, align, padding);
	}
	static inline void DrawFontsAtFixedSize(HDC hdc, const string& str, int px, int py, int size_x, int size_y, DIRECTION align = RIGHT, int padding = 0)
	{
		int length = (int)str.size();
		if (padding > 0) {
			assert(padding >= length);
			switch (align)
			{
			case RIGHT:
				px += (padding - length) * size_x;
				break;
			case CENTER:
				px += (padding - length) / 2;
				if ((padding - length) % 2)
					px += size_x / 2;
				break;
			}
		}
		for (int i = 0; i < length; ++i)
		{
			m_mapGlyph[str[i]]->DrawImageAtFixedSize(hdc, px, py, size_x, size_y);
			px += size_x;
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