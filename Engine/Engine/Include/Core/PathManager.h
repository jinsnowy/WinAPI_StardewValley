#pragma once
#include "../framework.h"
class PathManager
{
	DECLARE_SINGLE(PathManager)
private:
	unordered_map<string, wstring> m_mapPath;
	char m_strPath[MAX_PATH];
public:
	bool Init();
	bool CreatePath(const string& strKey, const wchar_t* pPath, const string& strBaseKey = ROOT_PATH);
	const wchar_t* FindPath(const string& strKey);
	const char* FindPathByMultiByte(const string& strKey);
	void FileClear(const char* fileName, const string& strBaseKey);
	FILE* FileOpen(const char* fileName, const string& strBaseKey, const char* mode);
};