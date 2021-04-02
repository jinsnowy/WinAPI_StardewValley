#pragma once
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <string>
#include <memory>
#include <map>
#include <conio.h>
#define PROBE_PERFORMANCE(child, parent) Profiler _hide(child, parent);
#define PRINT_PROFILE_RESULT Profiler::PrintProfileResult();
using namespace std;

class Profiler
{
private:
	class Log
	{
		friend class Profiler;
	public:
		const char* m_Logger = "";
		int m_frameCount = 0;
		size_t m_Elapsed = 0;
		size_t m_Calculated = 0;
	public:
		Log(const char* logger)
			:
			m_Logger(logger)
		{
		}
		float toMiliSeconds() const { return float(m_Calculated) / 1000.f; }
		void Stack(const size_t& elapse)
		{
			++m_frameCount;
			m_Elapsed += elapse;
			if (m_frameCount == m_MaxFrame)
			{
				m_frameCount = 0;
				m_Calculated = m_Elapsed / m_MaxFrame;
				m_Elapsed = 0;
			}
		}
	};
private:
	using TIME_POINT = std::chrono::steady_clock::time_point;
	using Clock = std::chrono::steady_clock;
	using Cache = unique_ptr<Log>;
	static unordered_map<string, Cache> m_Cache;
	static multimap<string, string> m_NameMap;
private:
	static string m_EmptyStr;
	static Cache m_nullCache;
	static constexpr int m_MaxFrame = 60;
	string m_tmp_logger;
	TIME_POINT m_tic = Clock::now();
private:
	Cache& GetCache(const string& logger)
	{
		if (m_Cache.find(logger) != m_Cache.end())
		{
			return m_Cache[logger];
		}
		return m_nullCache;
	}
public:
	explicit Profiler(const char* child, const char* parent = "")
	{
#ifdef _DEBUG
		m_tmp_logger = child;
		if (GetCache(m_tmp_logger) == m_nullCache)
		{
			m_NameMap.insert(make_pair(m_tmp_logger, string(parent)));
			m_Cache.emplace(m_tmp_logger, std::move(make_unique<Log>(child)));
		}
#endif
	}
	~Profiler()
	{
#ifdef _DEBUG
		size_t elapse = (size_t)chrono::duration_cast<chrono::microseconds>((Clock::now() - m_tic)).count();
		GetCache(m_tmp_logger)->Stack(elapse);
#endif
	}
	static void PrintProfileResult()
	{
#ifdef _DEBUG
		static int iCount = 0;
		++iCount;
		if (iCount == m_MaxFrame)
		{
			iCount = 0;
			_cprintf("================== Profile Result =========================\n");
			const auto& iterEnd = m_NameMap.end();;
			for (auto iter = m_NameMap.begin(); iter != iterEnd; ++iter)
			{
				float elapse = m_Cache[iter->first]->toMiliSeconds();
				if (iter->second.size() == 0)
				{
					_cprintf("---[%s] %.3fms\n", iter->first.c_str(), elapse);
				}
				else 
				{
					_cprintf("[%s/%s] takes %.3fms\n", iter->second.c_str(), iter->first.c_str(), elapse);
				}
			}
			_cprintf("\n");
		}
#endif
	}
};
