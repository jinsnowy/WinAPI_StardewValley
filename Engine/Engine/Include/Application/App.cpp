#include "App.h"
#include "Window.h"
#include "../Core/FrameTimer.h"
#include "../Core/PathManager.h"
#include "../Core/Camera.h"
#include "../Core/Input.h"
#include "../Collider/CollisionManager.h"
#include "../Scene/SceneManager.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/PrototypeManager.h"
#include "../Resources/Texture.h"
#include "../Object/StaticObj/GameManager.h"
#include "../Sound/SoundManager.h"
#include "../Object/Mouse.h"

size_t input_total_us = 0;
size_t update_total_us = 0;
size_t lateupdate_total_us = 0;
size_t collision_total_us = 0;
size_t draw_total_us = 0;
int app_frame_count = 0;
int frame_num = 100;

App::App()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(3298);
	AllocConsole();
#endif
}

App::~App()
{
#ifdef _DEBUG
	FreeConsole();
#endif
	SCENE_MANAGER->Release();
	CAMERA->Release();
	INPUT->Release();
	COLLISION_MANAGER->Release();
	PROTOTYPE_MANAGER->Release();
	RESOURCE_MANAGER->Release();
	SOUND_MANAGER->Release();
	PATH_MANAGER->Release();
	TIMER->Release();
	WINDOW->Release();
}

int App::Go()
{
	TIMER->Reset();
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				return -1;
			}
		}
		else {
			// Game Frame goes
			Process();
		}
	}

	return (int)msg.wParam;
}

void App::Init()
{
	if (!WINDOW->Init(1280, 720, L"My First Game"))
	{
		throw EXCEPT(L"Window init failed.\n");
	}

	if (!TIMER->Init(WINDOW->m_hWnd))
	{
		throw EXCEPT(L"Timer init failed.\n");
	}

	if (!PATH_MANAGER->Init())
	{
		throw EXCEPT(L"PathManager init failed.\n");
	}

	if (!RESOURCE_MANAGER->Init(WINDOW->m_hInst, WINDOW->m_hDC))
	{
		throw EXCEPT(L"ResourceManager init failed.\n");
	}

	if (!PROTOTYPE_MANAGER->Init())
	{
		throw EXCEPT(L"PrototypeManager init failed.\n");
	}

	if (!SOUND_MANAGER->Init())
	{
		throw EXCEPT(L"SoundManager init failed.\n");
	}

	if (!COLLISION_MANAGER->Init())
	{
		throw EXCEPT(L"Collision Manager init failed.\n");
	}

	if (!INPUT->Init(WINDOWHANDLE))
	{
		throw EXCEPT(L"Input init failed.\n");
	}

	// 씬 초기화 전에 카메라 초기화
	if (!CAMERA->Init(Pos(0.f, 0.f), GETRESOLUTION, RESOLUTION(1920, 1080)))
	{
		throw EXCEPT(L"Camera init failed.\n");
	}

	if (!SCENE_MANAGER->Init())
	{
		throw EXCEPT(L"SceneManager init failed.\n");
	}
}

void App::Process()
{
	const float dt = TIMER->Tick();

	auto tic = chrono::steady_clock::now();
	Input(dt);
	auto toc = chrono::steady_clock::now();
	input_total_us += chrono::duration_cast<chrono::microseconds>(toc - tic).count();

	tic = chrono::steady_clock::now();
	if(SCENECHANGE(Update(dt)))
	{
		SCENE_MANAGER->ChangeScene();
		TIMER->Reset();
		return;
	}
	toc = chrono::steady_clock::now();
	update_total_us += chrono::duration_cast<chrono::microseconds>(toc - tic).count();

	tic = chrono::steady_clock::now();
	if(SCENECHANGE(LateUpdate(dt)))
	{
		SCENE_MANAGER->ChangeScene();
		TIMER->Reset();
		return;
	}
	toc = chrono::steady_clock::now();
	lateupdate_total_us += chrono::duration_cast<chrono::microseconds>(toc - tic).count();

	tic = chrono::steady_clock::now();
	Collision(dt);
	toc = chrono::steady_clock::now();
	collision_total_us += chrono::duration_cast<chrono::microseconds>(toc - tic).count();

	tic = chrono::steady_clock::now();
	Draw(dt);
	toc = chrono::steady_clock::now();
	draw_total_us += chrono::duration_cast<chrono::microseconds>(toc - tic).count();

#ifdef  _DEBUG
	++app_frame_count;
	if (app_frame_count == frame_num)
	{
		app_frame_count = 0;
		input_total_us /= frame_num;
		update_total_us /= frame_num;
		lateupdate_total_us /= frame_num;
		collision_total_us /= frame_num;
		draw_total_us /= frame_num;
		_cprintf("Input      takes %f ms\n", (float)input_total_us / 1000.f);
		_cprintf("Update     takes %f ms\n", (float)update_total_us / 1000.f);
		_cprintf("LateUpdate takes %f ms\n", (float)lateupdate_total_us / 1000.f);
		_cprintf("Collision  takes %f ms\n", (float)collision_total_us / 1000.f);
		_cprintf("Draw       takes %f ms\n", (float)draw_total_us / 1000.f);
		input_total_us = 0;
		update_total_us = 0;
		lateupdate_total_us = 0;
		collision_total_us = 0;
		draw_total_us = 0;
	}
#endif //  _DEBUG
}

void App::Input(float dt)
{
	INPUT->Update(dt);
	SCENE_MANAGER->Input(dt);
	CAMERA->Input(dt);
}

int App::Update(float dt)
{
	int res;
	res = SCENE_MANAGER->Update(dt);
	CAMERA->Update(dt);
	SOUND_MANAGER->Update(dt);
	return res;
}

int App::LateUpdate(float dt)
{
	int res = SCENE_MANAGER->LateUpdate(dt);
	return res;
}

void App::Collision(float dt)
{
	SCENE_MANAGER->Collision(dt);
	COLLISION_MANAGER->Collision2(dt);
}

void App::Draw(float dt)
{
	// 더블 버퍼링
	Texture* pBackBuffer = RESOURCE_MANAGER->GetBackBuffer();

	SCENE_MANAGER->Draw(pBackBuffer->GetDC(), dt);

	BitBlt(WINDOW->m_hDC, 0, 0, GETRESOLUTION.x, GETRESOLUTION.y, pBackBuffer->GetDC(), 0, 0, SRCCOPY);

	SAFE_RELEASE(pBackBuffer);

	Mouse* pMouse = INPUT->GetMouse();

	pMouse->Draw(WINDOW->m_hDC, dt);
}