#pragma once
#include "Base/Main/Common.h"

namespace S2DE::Math
{
	template<class T>
	static T Min(T a, T b)
	{
		return std::min(a, b);
	}
}