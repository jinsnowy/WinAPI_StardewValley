#include "Profiler.h"

unordered_map<string, Profiler::Cache> Profiler::m_Cache;
multimap<string, string> Profiler::m_NameMap;

string Profiler::m_EmptyStr = "";
Profiler::Cache Profiler::m_nullCache = nullptr;