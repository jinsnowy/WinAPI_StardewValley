#pragma once
#include "../framework.h"

class Input
{
	DECLARE_SINGLE(Input)
private:
	friend class Window;
	class KeyInfo
	{
		friend class Input;
	private:
		bool	bDown;  // 제일 처음 눌렀을 때 (true, false, false)
		bool	bPress; // 키 계속 눌렀을 때 (false, true, false)
		bool	bUp;    // 키 뗏을 때 (false, false, true)
		vector<DWORD> vecKey; // 조합 키
		string strName;
		KeyInfo():
			bDown(false),
			bPress(false),
			bUp(false)
		{
		}
	};
private:
	unordered_map<char, bool> m_unCatchedKeyMap;
	HWND							m_hWnd;
	unordered_map<string, KeyInfo*> m_mapKey;
	KeyInfo*						m_pCreateKey;
	POINT							m_tMousePos;
	POINT							m_tMouseMove;
	class Mouse* m_pMouse;
private:
	void SetUnCatchedKey(char c) { m_unCatchedKeyMap[c] = true; }
	void RegisterUnCatchedKey(char c) { m_unCatchedKeyMap.insert(make_pair(c, false)); }
	void InitUnCatchedKeyState();
	bool IsUnCatchedKey(char c) { return m_unCatchedKeyMap.find(c) != m_unCatchedKeyMap.end(); }
public:
	bool Init(HWND hWnd);
	void Update(float dt);
	bool KeyDown(const string& strKey) const;
	bool KeyPress(const string& strKey) const;
	bool KeyUp(const string& strKey) const;
	class Mouse* GetMouse() const { return m_pMouse; } 
	Pos GetMouseClientPos() const;
	Pos GetMouseWorldPos() const;
	Pos GetMouseMove() const;

public:
	template<typename T>
	bool AddKey(const T& data)
	{
		const char* pTType = typeid(T).name();
		if (strcmp(pTType, "char") == 0 || strcmp(pTType, "int") == 0)
		{
			m_pCreateKey->vecKey.push_back((DWORD)data);
		}
		else 
		{
			m_pCreateKey->strName = data;
			m_mapKey.insert(make_pair(m_pCreateKey->strName, m_pCreateKey));
		}
		return true;
	}

	template<typename T, typename... Types> // 가변 인자 템플릿
	bool AddKey(const T& data, const Types&... args)
	{
		if (!m_pCreateKey)
		{
			m_pCreateKey = new KeyInfo;
		}

		const char* pTType = typeid(T).name();
		if (strcmp(pTType, "char") == 0 || strcmp(pTType, "int") == 0)
		{
			m_pCreateKey->vecKey.push_back((DWORD)data);
		}
		else 
		{
			m_pCreateKey->strName = data;
			m_mapKey.insert(make_pair(m_pCreateKey->strName, m_pCreateKey));
		}
		AddKey(args...);

		if (m_pCreateKey)
			m_pCreateKey = nullptr;

		return true;
	}
	void DeleteKey(const string& strKey);
	KeyInfo* FindKey(const string& strKey) const;
};