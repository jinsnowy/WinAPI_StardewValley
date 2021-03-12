/******************************************************************************************
*	Chili Direct3D Engine																  *
*	Copyright 2018 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of Chili Direct3D Engine.											  *
*																						  *
*	Chili Direct3D Engine is free software: you can redistribute it and/or modify		  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili Direct3D Engine is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili Direct3D Engine.  If not, see <http://www.gnu.org/licenses/>.    *
******************************************************************************************/

#include "Window.h"
#include "../Resource.h"
#include "../Other/WindowsMessageMap.h"
#include "../Core/Input.h"
DEFINITION_SINGLE(Window)

Window::~Window()
{
	ReleaseDC(m_hWnd, m_hDC);
	UnregisterClass(GetName(), m_hInst);
	DestroyWindow(m_hWnd);
}

Window::Window() 
	: m_hInst(GetModuleHandle(nullptr)),
	  m_hWnd(nullptr),
	  m_hDC(nullptr),
	  m_RS(800, 600)
{
}

BOOL Window::Init(int width, int height, const wchar_t* name)
{
	BOOL init = TRUE;
	m_RS = { width, height };

	MyRegisterClass();

	m_hWnd = CreateWindow(GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		0, 0, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, m_hInst, this);

	if (m_hWnd == nullptr)
	{
		throw WND_LAST_EXCEPT();
	}

	m_hDC = GetDC(m_hWnd);

	if (m_hDC == nullptr)
	{
		throw WND_LAST_EXCEPT();
	}

	RECT wr = { 0,0,width, height };
	init &= AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	init &= SetWindowPos(m_hWnd, HWND_TOPMOST, 100, 100, wr.right - wr.left, wr.bottom - wr.top, SWP_NOZORDER);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	return init;
}

void Window::DestroyGame()
{
	DestroyWindow(m_hWnd);
}

void Window::MyRegisterClass()
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInst;
	wc.hIcon = static_cast<HICON>(LoadImage(m_hInst, MAKEINTRESOURCE(IDI_ENGINE), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(m_hInst, MAKEINTRESOURCE(IDI_SMALL), IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&wc);

	if (RegisterWindowMessage(L"WM_RENDER_RESET") == 0)
	{
		throw WND_LAST_EXCEPT();
	}
}

void Window::SetTitle(const wstring& title)
{
	if (SetWindowText(m_hWnd, title.c_str()) == 0)
	{
		throw WND_LAST_EXCEPT();
	}
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND m_hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window instance handler
		return pWnd->HandleMsg(m_hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(m_hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND m_hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window instance
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(m_hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(m_hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND m_hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	
	// static WindowsMessageMap mm;
	// OutputDebugString(GetWChar(mm(msg, lParam, wParam).c_str()));

	// ¸¶¿ì½º
	const POINTS pt = MAKEPOINTS(lParam);

	switch (msg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CHAR:
		if (INPUT->IsUnCatchedKey(char(wParam)))
		{
			INPUT->SetUnCatchedKey(char(wParam));
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hWnd, &ps);

		EndPaint(m_hWnd, &ps);
	}break;
	}
	return DefWindowProc(m_hWnd, msg, wParam, lParam);
}

Window::WindowException::WindowException(int codeLine, const char* fileName, HRESULT hr) noexcept
	:
	UserException(codeLine, fileName),
	hr(hr)
{
}

const wchar_t* Window::WindowException::GetType() const noexcept
{
	return L"User Window WindowException";
}

wstring Window::WindowException::GetErrorMessage() const noexcept
{
	wstringstream oss;
	oss << Window::WindowException::UserException::GetErrorMessage()
		<< "[Error Code] " << hr << endl
		<< "[Description] " << TranslateErrorCode(hr) << endl;
	return oss.str();
}

wstring Window::WindowException::TranslateErrorCode(HRESULT hr) noexcept
{
	wchar_t* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&pMsgBuf), 0, nullptr);

	if (nMsgLen == 0)
	{
		return L"Unidentified error code";
	}
	wstring errorString = pMsgBuf;
	return errorString;
}
