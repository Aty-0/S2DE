#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <typeindex>
#include <string>
#include <utility>
#include <vector>

#include <windows.h>

//Disable warning "needs to have dll-interface to be used by clients of class"
#pragma warning(disable: 4251)

//S2DE
#include "Base/Main/S2DE_API.h"
#include "Base/Utils/StringUtils.h"


//DX11
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcommon.h>

#include <DirectXMath.h>
#include "SimpleMath.h"

#include <comdef.h>

#define S2DE_ALIGN(s) __declspec(align(s))

namespace S2DE::Core
{
	template<typename T>
	static void Delete(T t)
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
}

namespace S2DE::Render
{
	struct S2DE_API Vertex
	{
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Vector4 color;
		DirectX::SimpleMath::Vector2 uv;
	};
}



