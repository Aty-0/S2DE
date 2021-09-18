#pragma once
#include "Base/Main/Common.h"

namespace S2DE
{
	class S2DE_API FloatRect
	{
	public:
		FloatRect() = default;
		explicit FloatRect(float left, float top, float width, float height);

		bool operator !=(FloatRect r) const noexcept;
		bool operator ==(FloatRect  r) const noexcept;
		void operator =(FloatRect  r);

		static RECT inRECT(FloatRect rect)  noexcept
		{
			RECT r;
			r.bottom = (LONG)rect.height;
			r.right = (LONG)rect.width;
			r.left = (LONG)rect.left;
			r.top = (LONG)rect.top;

			return r;
		}
		
		float left = 0.0f;
		float top = 0.0f;
		float height = 0.0f;
		float width = 0.0f;
	};

	class S2DE_API  IntRect
	{
	public:
		IntRect() = default;
		explicit IntRect(std::int32_t left, std::int32_t top, std::int32_t width, std::int32_t height);
		bool operator !=(IntRect r) const noexcept;
		bool operator ==(IntRect r) const noexcept;
		void operator =(IntRect r);

		static RECT inRECT(IntRect rect) noexcept
		{
			RECT r;
			r.bottom = (LONG)rect.height;
			r.right = (LONG)rect.width;
			r.left = (LONG)rect.left;
			r.top = (LONG)rect.top;

			return r;
		}

		std::int32_t left = 0;
		std::int32_t top = 0;
		std::int32_t height = 0;
		std::int32_t width = 0;

    };
}