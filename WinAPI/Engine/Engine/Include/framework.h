// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#define NOMINMAX
// Windows 헤더 파일
#include <windows.h>
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

// 매크로
#define TIME_UNIT 7.166f
#define SWINGSPEED 0.3f
#define TOOLSPEED 0.6f
#define GLYPHX 20
#define GLYPHY 24
#define PI		3.141592f
#define GRAVITY 600.f
#define TILESIZE 64
#define RAD2DEG(X) (X/PI*180.0f)
#define DEG2RAD(X) (X*PI/180.f)
#define HOURS(X) (X*6)
#define DAYS(X)  (HOURS(24*X))
// Path Key
#define ROOT_PATH		"RootPath"
#define TEXTURE_PATH	"TexturePath"
#define DATA_PATH		"DataPath"
#define SOUND_PATH		"SoundPath"

#define WM_RENDER_RESET (WM_USER + 1)

#define WINDOW Window::Instance()
#define WINDOWHANDLE Window::Instance()->GetWindowHandle()
#define WINDOWINSTANCE Window::Instance()->GetWindowInstance()
#define GETRESOLUTION Window::Instance()->GetResolution()
#define GETDC Window::Instance()->GetWndDC()

#define COLLISION_MANAGER CollisionManager::Instance()
#define TIMER FrameTimer::Instance()
#define SCENE_MANAGER SceneManager::Instance()
#define PATH_MANAGER PathManager::Instance()
#define PROTOTYPE_MANAGER PrototypeManager::Instance()
#define RESOURCE_MANAGER ResourceManager::Instance()
#define CAMERA Camera::Instance()
#define INPUT Input::Instance()
#define SOUND_MANAGER SoundManager::Instance()
#define GAME_MANAGER GameManager::Instance()


// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <conio.h>
#include <iomanip>
#include <assert.h>
#include <string.h>
#include <stdarg.h> 

// C++ 헤더 파일
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <typeinfo>
#include <filesystem>
#include <istream>
#include <fstream>
#include <utility>
#include <chrono>

// 자료구조
#include <bitset>
#include <vector>
#include <queue>
#include <deque>
#include <list>
#include <array>
#include <unordered_map>

// 알고리즘
#include <algorithm>
#include <functional>
#include <random>
#include <unordered_set>

#include "Types/Vec2.h"
#include "Types/Rect.h"
#include "Types/Circle.h"
#include "Types/Types.h"
#include "Resource.h"
#include "Macro.h"
#include "utils.h"
#include "Math.h"
#include "Other/UserException.h"

// 동적 링크 라이브러리
#pragma comment(lib, "msimg32")
#pragma comment(lib, "fmod_vc")

using namespace std;
using namespace util;
namespace fs = std::filesystem;
typedef Vec2F Size;
typedef Vec2F Pos;
typedef Vec2I RESOLUTION;
typedef Vec2I INDEX;
typedef RectF Rect;
typedef CircleF Sphere;

template <typename T>
void Safe_Delete_VecList(T& p)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}
	p.clear();
}

template <typename T>
void Safe_Release_VecList(T& p)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE((*iter));
	}
	p.clear();
}

template <typename T>
void Safe_Delete_Map(T& p)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second)
	}
	p.clear();
}

template <typename T>
void Safe_Release_Map(T& p)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second);
	}
	p.clear();
}
