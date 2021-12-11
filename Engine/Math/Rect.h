#pragma once
#include "Base/Main/Common.h"

namespace S2DE::Math
{
	template<typename T>
	class S2DE_API Rect
	{
	public:
		Rect() = default;
		explicit Rect(T left, T top, T width, T height)
		{
			this->left = left;
			this->top = top;
			this->height = height;
			this->width = width;
		}

		inline bool operator !=(const Rect& r) const noexcept
		{
			return !(left == r.left && top == r.top && height == r.height && width == r.width);
		}

		inline bool operator ==(const Rect& r) const noexcept
		{
			return (left == r.left && top == r.top && height == r.height && width == r.width);
		}

		
		static RECT ToWinRect(Rect rect) noexcept
		{
			RECT r;
			r.bottom = (LONG)rect.height;
			r.right =  (LONG)rect.width;
			r.left =   (LONG)rect.left;
			r.top =    (LONG)rect.top;

			return r;
		}

		T left = 0;
		T top = 0;
		T height = 0;
		T width = 0;

    };
}