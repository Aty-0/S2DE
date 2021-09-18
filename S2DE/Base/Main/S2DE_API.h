#pragma once
#ifdef S2DE_EXPORTS
#define S2DE_API __declspec(dllexport)
#else
#define S2DE_API __declspec(dllimport)
#endif

#define S2DE_NAME_DLL "S2DE.dll"