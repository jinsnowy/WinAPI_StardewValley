#pragma once
#include <string>
#include <string.h>
#include <sstream>
#include <memory>
#include <windows.h>
#include <random>
using namespace std;
namespace util
{
	static char _char_buffer[MAX_PATH] = {0};
	static wchar_t _wchar_buffer[MAX_PATH] = {0};
	static random_device _rnd;
	static mt19937 _rng(_rnd());
	static uniform_real_distribution<float> _real_dist;
	static uniform_int_distribution<int> _int_dist;

	const wchar_t* GetWChar(const char* c);
	const char* GetChar(const wchar_t* c);
	inline RECT MakeRect(int left, int top, int w, int h)
	{
		RECT rc = {};
		rc.left = left;
		rc.top = top;
		rc.right = rc.left + w;
		rc.bottom = rc.top + h;
		return rc;
	}
	uniform_real_distribution<float> GenUniformRealDist(float lower, float upper);
	uniform_int_distribution<int> GenUniformIntDist(int lower, int upper);
	void SetUniformRealDist(float lower, float upper);
	void SetUniformIntDist(int lower, int upper);
	float GenerateFloatNumber();
	int GenerateIntNumber();

	void DrawRedRect(HDC hdc, RECT rc);
	void DrawVoidRectWithBorderColor(HDC hdc, RECT rc, COLORREF color);
	void DrawColorRectWithOutBorder(HDC hdc, RECT rc, COLORREF color);
	void DrawHDCWithColor(HDC hdc, int w, int h, COLORREF color);
	void DrawHDCWithColor(HDC hdc, int px, int py, int w, int h, COLORREF color);
	std::string ExtractKeyFromPathString(const char* str, int size);
}
