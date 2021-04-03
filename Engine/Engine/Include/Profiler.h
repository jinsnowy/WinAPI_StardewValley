#pragma once
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <string>
#include <memory>
#include <map>
#include <conio.h>
#include <set>
#include <algorithm>
#define PROBE_PERFORMANCE(tag) Profiler _hide(tag);
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
		size_t m_Elapsed = 0;
		size_t m_Calculated = 0;
	public:
		Log(const char* logger)
			:
			m_Logger(logger)
		{
		}
		float toMiliSeconds() const
		{
			return float(m_Calculated) / 1000.f;
		}
		void Update()
		{
			m_Calculated = m_Elapsed / m_MaxFrame;
			m_Elapsed = 0;
		}
		void Stack(const size_t& elapse)
		{
			m_Elapsed += elapse;
		}
	};
private:
	using TIME_POINT = std::chrono::steady_clock::time_point;
	using Clock = std::chrono::steady_clock;
	using Cache = unique_ptr<Log>;
	static unordered_map<string, Cache> m_Cache;
	static set<string> m_NameMap;
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
			m_NameMap.insert(string(logger));
			return m_Cache[logger];
		}
		return m_nullCache;
	}
public:
	explicit Profiler(const char* tag)
	{
#ifdef _DEBUG
		m_tmp_logger = tag;
		if (GetCache(m_tmp_logger) == m_nullCache)
		{
			m_Cache.emplace(m_tmp_logger, std::move(make_unique<Log>(tag)));
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
	static void PrintProfileResult();
};
