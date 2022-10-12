#pragma once
#include <windows.h>
#include <psapi.h>

namespace S2DE::Core::Utils
{
	static const std::uint64_t GetUsedMem()
	{
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		
		return std::uint64_t(pmc.PagefileUsage >> 20);
	}	
}