#pragma once
#include "Base/Main/Common.h"
#include "Graphics/Renderer.h"


namespace S2DE
{
	template<typename T = std::int32_t>
    class S2DE_API Color
	{
	public:
		Color() = default;
		Color(T r, T g, T b, T a)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}


		T r, g, b, a;
	};
}