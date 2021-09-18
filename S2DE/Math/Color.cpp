#include "Color.h"


namespace S2DE
{
	Color::Color(const Color& color)
	{
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
	}

	Color::Color(std::int32_t cr, std::int32_t cg, std::int32_t cb, std::int32_t ca)
	{
		r = cr;
		g = cg;
		b = cb;
		a = ca;
	}
}