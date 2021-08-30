#pragma once
#ifdef S2DE_DX11_EXPORTS
#define S2DE_DX11_API __declspec(dllexport)
#else
#define S2DE_DX11_API __declspec(dllimport)
#endif
