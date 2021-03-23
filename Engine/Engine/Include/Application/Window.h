#pragma once
#include "../framework.h"
#include "../Other/UserException.h"

// error exception helper macro
#define WND_EXCEPT(hr) Window::WindowException(__LINE__, __FILE__, hr);
#define WND_LAST_EXCEPT() Window::WindowException(__LINE__, __FILE__, GetLastError());

class Window
{
	DECLARE_SINGLE(Window)
private:
	class WindowException : public UserException
	{
	public:
		WindowException(int codeLine, const char* fileName, HRESULT hr) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
		virtual wstring GetErrorMessage() const noexcept override;
	private:
		HRESULT hr;
		static wstring TranslateErrorCode(HRESULT hr) noexcept;
	};
private:
	static constexpr const wchar_t* wndClassName = L"myEngine";
private:
	HINSTANCE m_hInst;
	HWND m_hWnd;
	HDC m_hDC;
	RESOLUTION m_RS;
public:
	BOOL Init(int width, int height, const wchar_t* name);
	HINSTANCE GetWindowInstance() { return m_hInst; }
	HWND GetWindowHandle() { return m_hWnd; }
	HDC GetWndDC() { return m_hDC; }
	RESOLUTION GetResolution() noexcept { return m_RS; }
	static const wchar_t* GetName() { return wndClassName; }
	void DestroyGame();
private:
	friend class App;
	void MyRegisterClass();
	void SetTitle(const wstring& title);

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
};

