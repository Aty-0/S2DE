#pragma once
#include "Math/Min.h"
#include "Math/Max.h"

namespace S2DE::Math
{
	template<class T>
	static T Clamp(T x, T upper, T lower)
	{
		return Min(upper, Max(x, lower));
	}
}