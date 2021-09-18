#pragma once
#include "Base/Main/Common.h"
#include "Graphics/Renderer.h"


namespace S2DE
{
    class S2DE_API Color
	{
	public:
		Color() = default;
		Color(const Color& color);
		Color(std::int32_t cr, std::int32_t cg, std::int32_t cb, std::int32_t ca);

		std::int32_t r, g, b, a;

		//Color presets
		static const inline Color White() { return Color(255, 255, 255, 255); }
		static const inline Color Black() { return Color(0, 0, 0, 255); }
		static const inline Color Blue() { return Color(0, 0, 255, 255); }
		static const inline Color Red() { return Color(255, 0, 0, 255); }
		static const inline Color Green() { return Color(0, 255, 0, 255); }
	};
}