#include "Input.h"
#include "../Object/Mouse.h"
#include "../Animation/Animation.h"
#include "../Collider/ColliderPoint.h"
#include "../Collider/CollisionManager.h"

DEFINITION_SINGLE(Input)
Input::Input()
	: m_hWnd(nullptr),
	  m_pCreateKey(nullptr),
	  m_pMouse(nullptr),
	  m_tMouseMove(),
	  m_tMousePos()
{
}

Input::~Input()
{
	SAFE_RELEASE(m_pMouse);
	Safe_Delete_Map(m_mapKey);
}

void Input::InitUnCatchedKeyState()
{
	for (auto iter = m_unCatchedKeyMap.begin(); iter != m_unCatchedKeyMap.end(); ++iter)
	{
		iter->second = false;
	}
}

bool Input::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	AddKey("MoveUp", 'W');
	AddKey("MoveDown", 'S');
	AddKey("MoveLeft", 'A');
	AddKey("MoveRight", 'D');

	AddKey(VK_LBUTTON, "MouseLButton");
	AddKey(VK_RBUTTON, "MouseRButton");

	RegisterUnCatchedKey('-');
	RegisterUnCatchedKey('=');

	// 마우스 위치를 얻어오는 함수이다.
	GetCursorPos(&m_tMousePos);

	// 마우스 생성
	m_pMouse = Object::CreateObject<Mouse>("Mouse");


	m_pMouse->SetTexture("MouseTexture", L"mouse.bmp");
	m_pMouse->SetSize(32, 19);
	m_pMouse->SetColorKey(255, 255, 255);

	return true;
}

void Input::Update(float dt)
{
	auto iterEnd = m_mapKey.end();
	for (auto iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		int iPushCount = 0;
		for (size_t i = 0; i < iter->second->vecKey.size(); ++i)
		{
			const char& key = iter->second->vecKey[i];
			if (IsUnCatchedKey(key))
			{
				if (m_unCatchedKeyMap[key])
				{
					iPushCount++;
				}
			}
			else if (GetAsyncKeyState(key) & 0x8000)
			{
				iPushCount++;
			}
		}

		if (iPushCount == iter->second->vecKey.size())
		{
			// 둘 다 안 눌렸었던 상태
			if (!iter->second->bDown && !iter->second->bPress)
			{
				iter->second->bPress = true;
				iter->second->bDown = true;
			}
			// 한 번 눌려졌었던 상태
			else if (iter->second->bDown)
			{
				iter->second->bDown = false;
			}
		}
		else {
			// 키를 뗏음
			// 눌려졌었음
			if (iter->second->bDown || iter->second->bPress)
			{
				iter->second->bDown = false;
				iter->second->bPress = false;
				iter->second->bUp = true;
			}
			// 뗀 상태 였음
			else if (iter->second->bUp)
			{
				iter->second->bUp = false;
			}
		}
	}

	m_iMouseScroll = 0;
	if (m_vecScroll.size())
	{
		for (int delta : m_vecScroll) m_iMouseScroll += delta;
		m_vecScroll.clear();
	}

	m_pMouse->Update(dt);
	m_pMouse->LateUpdate(dt);

	COLLISION_MANAGER->AddObject(m_pMouse);

	InitUnCatchedKeyState();
}

bool Input::KeyDown(const string& strKey) const
{
	KeyInfo* pInfo = FindKey(strKey);
	if (!pInfo)
		return false;
	return pInfo->bDown;
}

bool Input::KeyPress(const string& strKey) const
{
	KeyInfo* pInfo = FindKey(strKey);
	if (!pInfo)
		return false;
	return pInfo->bPress;
}

bool Input::KeyUp(const string& strKey) const
{
	KeyInfo* pInfo = FindKey(strKey);
	if (!pInfo)
		return false;
	return pInfo->bUp;
}

Pos Input::GetMouseClientPos() const
{
	return m_pMouse->GetPos();
}

Pos Input::GetMouseWorldPos() const
{
	return m_pMouse->GetWorldPos();
}

Pos Input::GetMouseMove() const
{
	return m_pMouse->GetMove();
}

void Input::DeleteKey(const string& strKey)
{
	if (FindKey(strKey))
	{
		SAFE_DELETE(m_mapKey[strKey]);
		m_mapKey.erase(strKey);
	}
}

Input::KeyInfo* Input::FindKey(const string& strKey) const
{
	const auto iter = m_mapKey.find(strKey);
	if (iter == m_mapKey.end())
		return nullptr;
	return iter->second;
}