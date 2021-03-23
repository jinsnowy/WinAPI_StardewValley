#include "PathManager.h"

DEFINITION_SINGLE(PathManager)

PathManager::PathManager()
{
}

PathManager::~PathManager()
{
}

bool PathManager::Init()
{
	wchar_t strPath[MAX_PATH] = {};

	// 현재 디렉토리 기준 실행 파일이 있는 폴더를 구한다.
	GetModuleFileName(NULL, strPath, MAX_PATH);

	// 루트 경로 찾기
	bool skip = true;
	for (int i = lstrlen(strPath) - 1; i >= 0; --i)
	{
		if (strPath[i] == '/' || strPath[i] == '\\')
		{
			if (skip)
			{
				skip = false;
				continue;
			}
			memset(strPath + (i + 1), 0, sizeof(wchar_t) * (MAX_PATH - (i + 1)));
			break;
		}
	}

	m_mapPath.insert(make_pair(ROOT_PATH, strPath));

	// Texture 경로 설정
	if (!CreatePath(TEXTURE_PATH, L"Texture\\"))
		return false;

	// Data 경로 설정
	if (!CreatePath(DATA_PATH, L"Data\\"))
		return false;

	// Sound 경로 설정
	if (!CreatePath(SOUND_PATH, L"Sounds\\"))
		return false;

	return true;
}

bool PathManager::CreatePath(const string& strKey, const wchar_t* pPath, const string& strBaseKey)
{
	const wchar_t* pBasePath = FindPath(strBaseKey);

	wstring strPath;
	if (pBasePath)
		strPath = pBasePath;

	strPath += pPath;

	m_mapPath.insert(make_pair(strKey, strPath));

	return true;
}

const wchar_t* PathManager::FindPath(const string& strKey)
{
	auto iter = m_mapPath.find(strKey);
	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second.c_str();
}

const char* PathManager::FindPathByMultiByte(const string& strKey)
{
	const wchar_t* pPath = FindPath(strKey);

	if (!pPath)
		return NULL;

	memset(m_strPath, 0, sizeof(char) * MAX_PATH);

	WideCharToMultiByte(CP_ACP, 0, pPath, - 1, m_strPath, lstrlen(pPath), 0, 0);

	return m_strPath;
}

void PathManager::FileClear(const char* fileName, const string& strBaseKey)
{
	const char* pPath = PATH_MANAGER->FindPathByMultiByte(strBaseKey);

	string strFullPath;
	if (pPath)
		strFullPath = pPath;
	strFullPath += fileName;

	FILE* pFile = NULL;

	fopen_s(&pFile, strFullPath.c_str(), "wb");

	if (pFile)
	{
		fclose(pFile);
	}
}

FILE* PathManager::FileOpen(const char* fileName, const string& strBaseKey, const char* mode)
{
	const char* pPath = PATH_MANAGER->FindPathByMultiByte(strBaseKey);

	string strFullPath;
	if (pPath)
		strFullPath = pPath;
	strFullPath += fileName;

	FILE* pFile = NULL;

	fopen_s(&pFile, strFullPath.c_str(), mode);

	return pFile;
}
