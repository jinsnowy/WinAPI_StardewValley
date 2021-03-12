#include "utils.h"
#include "Resources/ResourceManager.h"

const wchar_t* util::GetWChar(const char* c)
{
	size_t cSize = strlen(c) + 1;
	memset(_wchar_buffer, 0, sizeof(_wchar_buffer));
	mbstowcs_s(&cSize, _wchar_buffer, c, MAX_PATH);
	// mbstowcs(_wchar_buffer, c, cSize);
	return _wchar_buffer;
}

const char* util::GetChar(const wchar_t* c)
{
	int cSize = lstrlen(c) + 1;
	memset(_char_buffer, 0, sizeof(_char_buffer));
	WideCharToMultiByte(CP_ACP, 0, c, -1, _char_buffer, lstrlen(c), 0, 0);
	return _char_buffer;
}

void util::DrawRedRect(HDC hdc, RECT rc)
{
	HPEN myPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN OldPen = (HPEN)SelectObject(hdc, myPen);
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
	DeleteObject(SelectObject(hdc, OldPen));
	DeleteObject(SelectObject(hdc, OldBrush));
}

void util::DrawRectWithColor(HDC hdc, RECT rc, COLORREF color)
{
	HPEN myPen = CreatePen(PS_SOLID, 3, color);
	HPEN OldPen = (HPEN)SelectObject(hdc, myPen);
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
	DeleteObject(SelectObject(hdc, OldPen));
	DeleteObject(SelectObject(hdc, OldBrush));
}

void util::DrawHDCWithColor(HDC hdc, int w, int h, COLORREF color)
{
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(color));

	Rectangle(hdc, -5, -5, w+5, h+5);

	DeleteObject(SelectObject(hdc, OldBrush));
}

void util::DrawHDCWithColor(HDC hdc, int px, int py, int w, int h, COLORREF color)
{
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(color));

	Rectangle(hdc, px-5, py-5, px + w + 5, py + h + 5);

	DeleteObject(SelectObject(hdc, OldBrush));
}

std::string util::ExtractKeyFromPathString(const char* str, int size)
{
	int ed = size - 1;
	while (str[ed] != L'.') --ed;
	int st = ed - 1;
	while (str[st] != L'\\' && str[st] != L'/') st--;
	return std::string(str + st + 1, str + ed);
}
