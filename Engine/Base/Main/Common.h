#pragma once
#include <iostream>
#include <map>
#include <typeindex>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>

#include <typeinfo>
#include <typeindex>

#define NOMINMAX
#include <windows.h>

// Disable warning "needs to have dll-interface to be used by clients of class"
#pragma warning(disable: 4251)

// Some extra utils 
#include "Base/Main/S2DE_API.h"
#include "Base/Utils/StringUtils.h"
#include "Base/Utils/Singleton.h"

// SDL
#include "SDL.h"
#include "SDL_syswm.h"

// DX11
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcommon.h>

#include <comdef.h>

#include <DirectXMath.h>
#include "SimpleMath.h"
namespace S2DE::Math
{
	// HLSL-like type names
	using float2 = DirectX::SimpleMath::Vector2;
	using float3 = DirectX::SimpleMath::Vector3;
	using float4 = DirectX::SimpleMath::Vector4;
	using float4x4 = DirectX::SimpleMath::Matrix;
	using ulong = unsigned long;
}

namespace S2DE::Render
{
	struct S2DE_API Vertex
	{
		Math::float3 position;
		Math::float4 color;
		Math::float2 uv;
		Math::float3 normal;
	};
}

#define S2DE_ALIGN(s) __declspec(align(s))

namespace S2DE::Core
{
	template<typename T>
	static void Delete(T*& t) noexcept
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}

	template<typename T>
	static void Release(T*& t) noexcept
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}

	// For direct input
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




