#pragma once
#include <iostream>

//Disable warning "needs to have dll-interface to be used by clients of class"
#pragma warning(disable: 4251)
#pragma warning(disable: 4005)

//S2DE API
#include "S2DE_DX11_API.h"

#include <windows.h>

//////////////////////////////////////////////
//Dx11
//////////////////////////////////////////////
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "Base/Other/StringUtils.h"

namespace S2DE
{
	template<typename T>
	static void  Delete(T t)
	{
		if (t)
		{
			delete t;
			t = NULL;
		}
	}

	template<typename T>
	static void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}

	//For direct input
	template<typename T>
	static void ReleaseInputDevice(T t)
	{
		if (t)
		{
			t->Unacquire();
			t->Release();
			t = nullptr;
		}
	}

	struct S2DE_DX11_API VideoCardDesc
	{
		std::int32_t Memory;
		char Description[128];
	};
}

