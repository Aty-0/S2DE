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
#include "Base/Other/StringUtils.h"


//DX11
#include <dxgi.h>
#include <d3d11_1.h>
#include <d3dcommon.h>

#include <DirectXMath.h>


#include <comdef.h>

#define S2DE_ALIGN(s) __declspec(align(s))

namespace S2DE::Core
{
	template<typename T>
	static void  Delete(T t)
	{
		if (t)
		{
			//Debug stuff
#if 0
			OutputDebugString(std::string("\nDeleted " + std::string(typeid(t).name()) + "\n").c_str());
#endif 

			delete t;
			t = NULL;
		}
	}

	template<typename T>
	static void Release(T t)
	{
		if (t)
		{
			//Debug stuff
#if 0
			OutputDebugString(std::string("\nReleased " + std::string(typeid(t).name()) + "\n").c_str());
#endif 
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

namespace S2DE::Math
{
	//DirectX Math library defines
	typedef S2DE_API DirectX::XMVECTOR XVector;
	typedef S2DE_API DirectX::XMMATRIX XMatrix;

	typedef S2DE_API DirectX::XMINT2 XInt2;
	typedef S2DE_API DirectX::XMINT3 XInt3;
	typedef S2DE_API DirectX::XMINT4 XInt4;

	typedef S2DE_API DirectX::XMFLOAT2 XFloat2;
	typedef S2DE_API DirectX::XMFLOAT3 XFloat3;
	typedef S2DE_API DirectX::XMFLOAT4 XFloat4;

	typedef S2DE_API DirectX::XMFLOAT4X4 XMatrix4x4;
	typedef S2DE_API DirectX::XMFLOAT4X3 XMatrix4x3;
	typedef S2DE_API DirectX::XMFLOAT3X3 XMatrix3x3;
}

namespace S2DE::Render
{
	struct S2DE_API Vertex
	{
		Math::XFloat3 position;
		Math::XFloat4 color;
		Math::XFloat2 uv;
	};
}



