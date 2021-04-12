#include "Profiler.h"

unordered_map<string, Profiler::Cache> Profiler::m_Cache;
set<string> Profiler::m_NameMap;
Profiler::Cache Profiler::m_nullCache = nullptr;

void Profiler::PrintProfileResult()
{
#ifdef _DEBUG
	static int iCount = 0;
	++iCount;
	if (iCount == m_MaxFrame)
	{
		iCount = 0;
		system("cls");
		_cprintf("================== Profile Result =========================\n");
		const auto iterEnd = m_NameMap.end();
		for (auto iter = m_NameMap.begin(); iter != iterEnd; ++iter)
		{
			const auto& info = m_Cache[(*iter)];
			info->Update();
			float elapse = info->toMiliSeconds();
			_cprintf("[%s] takes %.3fms\n", (*iter).c_str(), elapse);
		}
		_cprintf("\n");
	}
#endif
}