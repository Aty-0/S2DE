#pragma once
#include <windows.h>
#include <psapi.h>

namespace S2DE
{
	static const DWORDLONG GetUsedMem()
	{
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		
		return DWORDLONG(pmc.PagefileUsage >> 20);
	}	
}