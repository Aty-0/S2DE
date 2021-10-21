#pragma once
#include "Base/Main/Common.h"

namespace S2DE
{
	template<class T>
	static T Min(T a, T b)
	{
		return min(a, b);
	}
}