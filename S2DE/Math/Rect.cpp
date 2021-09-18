#include "Rect.h"

namespace S2DE
{
	FloatRect::FloatRect(float left, float top, float width, float height)
	{
		this->left = left;
		this->top = top;
		this->height = height;
		this->width = width;
	}

	bool FloatRect::operator !=(FloatRect r) const noexcept
	{
		return !(left == r.left && top == r.top && height == r.height && width == r.width);
	}

	bool FloatRect::operator ==(FloatRect r) const noexcept
	{
		return (left == r.left && top == r.top && height == r.height && width == r.width);
	}

	void FloatRect::operator =(FloatRect r)
	{
		this->left = r.left;
		this->top = r.top;
		this->height = r.height;
		this->width = r.width;
	}

	IntRect::IntRect(std::int32_t left, std::int32_t top, std::int32_t width, std::int32_t height)
	{
		this->left = left;
		this->top = top;
		this->height = height;
		this->width = width;
	}

	bool IntRect::operator !=(IntRect r) const noexcept
	{
		return !(left == r.left && top == r.top && height == r.height && width == r.width);
	}

	bool IntRect::operator ==(IntRect r) const noexcept
	{
		return (left == r.left && top == r.top && height == r.height && width == r.width);
	}

	void IntRect::operator =(IntRect r) 
	{
		this->left = r.left;
		this->top = r.top;
		this->height = r.height;
		this->width = r.width;
	}
}