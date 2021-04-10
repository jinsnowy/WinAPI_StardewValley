#include "UnReleaseTest.h"
#include "Object/Object.h"

void GetUndeletedList()
{
#ifdef _DEBUG
	wchar_t msg[MAX_PATH] = {};
	const auto iterEnd = _undeleted_list.end();
	for (auto iter = _undeleted_list.begin(); iter != iterEnd; ++iter)
	{
		Object* pObj = reinterpret_cast<Object*>(*iter);
		const string& name = pObj->AccessTexture() ? pObj->AccessTexture()->GetTag() : pObj->GetTag();
		memset(msg, 0, sizeof(msg));
		wsprintf(msg, L"Memory Leaks : %s\n", GetWChar(name.c_str()));
		OutputDebugString(msg);
	}
#endif
}
