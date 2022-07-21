#pragma once
#include "Base/Main/Common.h"

namespace S2DE::Math
{
	template<typename T = std::int32_t>
    class S2DE_API Color
	{
	public:
		Color() :
			r(0), 
			g(0), 
			b(0), 
			a(0)
		{

		}

		Color(T r, T g, T b, T a)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

		~Color() 
		{
			r = 0;
			g = 0;
			b = 0;
			a = 0;
		}

		T r, g, b, a;
	};
}