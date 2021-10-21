#include "IntVector.h"

namespace S2DE
{
	IntVector2 IntVector2::operator = (const IntVector2& v) noexcept
	{
		x = v.x;
		y = v.y;

		return *this;
	}

	IntVector2 IntVector2::operator -= (const IntVector2& v) noexcept
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}

	IntVector2 IntVector2::operator += (const IntVector2& v) noexcept
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	IntVector2 IntVector2::operator / (const std::int32_t v) const noexcept
	{
		return  IntVector2(x / v, y / v);
	}

	IntVector2 IntVector2::operator / (const IntVector2& v) const noexcept
	{
		return  IntVector2(x / v.x, y / v.y);
	}

	IntVector2 IntVector2::operator * (const std::int32_t v) const noexcept
	{
		return  IntVector2(x * v, y * v);
	}

	IntVector2 IntVector2::operator * (const IntVector2& v) const noexcept
	{
		return  IntVector2(x * v.x, y * v.y);
	}

	IntVector2 IntVector2::operator + (const std::int32_t v) const noexcept
	{
		return  IntVector2(x + v, y + v);
	}

	IntVector2 IntVector2::operator - (const std::int32_t v) const noexcept
	{
		return  IntVector2(x - v, y - v);
	}

	IntVector2 IntVector2::operator + () const noexcept
	{
		return *this;
	}

	IntVector2 IntVector2::operator + (const IntVector2& v) const noexcept
	{
		return IntVector2(x + v.x, y + v.y);
	}

	IntVector2 IntVector2::operator - (const IntVector2& v) const noexcept
	{
		return IntVector2(x - v.x, y - v.y);
	}

	IntVector2 IntVector2::Reset() noexcept
	{
		return IntVector2(0, 0);
	}

	IntVector2 IntVector2::operator - () const noexcept
	{
		return IntVector2(-x, -y);
	}

	bool IntVector2::operator ==(const IntVector2& v) const noexcept
	{
		return (this->x == v.x && this->y == v.y);
	}

	bool IntVector2::operator !=(const IntVector2& v) const noexcept
	{
		return !(this->x == v.x && this->y == v.y);
	}

	IntVector2 IntVector2::Normalize(IntVector2 vec) noexcept
	{
		vec.x /= (std::int32_t)vec.Length();
		vec.y /= (std::int32_t)vec.Length();

		return vec;
	}

	float IntVector2::Length() const noexcept
	{
		return (float)sqrt(x * x + y * y);
	}

	bool IntVector2::Zero() const noexcept
	{
		return this->x == 0 && this->y == 0;
	}

	IntVector3 IntVector3::operator = (const IntVector3& v)  noexcept
	{
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	IntVector3 IntVector3::operator = (const IntVector2& v)  noexcept
	{
		x = v.x;
		y = v.y;

		return *this;
	}

	IntVector3 IntVector3::operator -= (const IntVector3& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	IntVector3 IntVector3::operator += (const IntVector3& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	IntVector3 IntVector3::operator / (const std::int32_t v) const noexcept
	{
		return  IntVector3(x / v, y / v, z / v);
	}

	IntVector3 IntVector3::operator / (const IntVector3& v) const noexcept
	{
		return  IntVector3(x / v.x, y / v.y, z / v.z);
	}

	IntVector3 IntVector3::operator * (const std::int32_t v) const noexcept
	{
		return  IntVector3(x * v, y * v, z * v);
	}

	IntVector3 IntVector3::operator * (const IntVector3& v) const noexcept
	{
		return IntVector3(x * v.x, y * v.y, z * v.z);
	}

	IntVector3 IntVector3::operator + (const std::int32_t v) const noexcept
	{
		return  IntVector3(x + v, y + v, z + v);
	}

	IntVector3 IntVector3::operator - (const std::int32_t v) const noexcept
	{
		return  IntVector3(x - v, y - v, z - v);
	}

	IntVector3 IntVector3::operator + () const noexcept
	{
		return *this;
	}

	IntVector3 IntVector3::operator + (const IntVector3& v) const noexcept
	{
		return IntVector3(x + v.x, y + v.y, z + v.z);
	}

	IntVector3 IntVector3::operator - (const IntVector3& v) const noexcept
	{
		return IntVector3(x - v.x, y - v.y, z - v.z);
	}

	IntVector3 IntVector3::operator - () const noexcept
	{
		return IntVector3(-x, -y, -z);
	}

	bool IntVector3::operator == (const IntVector3& v) const noexcept
	{
		return (this->x == v.y && this->y == v.y && this->z == v.z);
	}

	bool IntVector3::operator !=(const IntVector3& v) const noexcept
	{
		return !(this->x == v.x && this->y == v.y && this->z == v.z);
	}

	bool IntVector3::Zero() const noexcept
	{
		return this->x == 0 && this->y == 0 && this->z == 0;
	}

	IntVector3 IntVector3::Reset() noexcept
	{
		return IntVector3(0, 0, 0);
	}

	IntVector3 IntVector3::Normalize(IntVector3 vec) noexcept
	{
		vec.x /= (std::int32_t)vec.Length();
		vec.y /= (std::int32_t)vec.Length();
		vec.z /= (std::int32_t)vec.Length();

		return vec;
	}

	float IntVector3::Length() const noexcept
	{
		return (float)sqrt(x * x + y * y + z * z);
	}

	IntVector4 IntVector4::operator = (const IntVector4& v) noexcept
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;

		return *this;
	}

	IntVector4 IntVector4::operator -= (const IntVector4& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;

		return *this;
	}

	IntVector4 IntVector4::operator += (const IntVector4& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}

	IntVector4 IntVector4::operator / (const std::int32_t v) const noexcept
	{
		return  IntVector4(x / v, y / v, z / v, w / v);
	}

	IntVector4 IntVector4::operator / (const IntVector4& v) const noexcept
	{
		return  IntVector4(x / v.x, y / v.y, z / v.z, w / v.w);
	}

	IntVector4 IntVector4::operator + (const std::int32_t v) const noexcept
	{
		return  IntVector4(x + v, y + v, z + v, w + v);
	}

	IntVector4 IntVector4::operator - (const std::int32_t v) const noexcept
	{
		return  IntVector4(x - v, y - v, z - v, w - v);
	}

	IntVector4 IntVector4::operator + () const noexcept
	{
		return *this;
	}

	IntVector4 IntVector4::operator * (const std::int32_t v) const noexcept
	{
		return  IntVector4(x * v, y * v, z * v, w * v);
	}

	IntVector4 IntVector4::operator * (const IntVector4& v) const noexcept
	{
		return  IntVector4(x * v.x, y * v.y, z * v.z, w * v.w);
	}

	IntVector4 IntVector4::operator + (const IntVector4& v) const noexcept
	{
		return IntVector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	IntVector4 IntVector4::operator - (const IntVector4& v) const noexcept
	{
		return IntVector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	IntVector4 IntVector4::operator - () const noexcept
	{
		return IntVector4(-x, -y, -z, -w);
	}

	bool IntVector4::operator == (const IntVector4& v) const noexcept
	{
		return (this->x == v.y && this->y == v.y && this->z == v.z && this->w == v.w);
	}

	bool IntVector4::operator !=(const IntVector4& v) const noexcept
	{
		return !(this->x == v.y && this->y == v.y && this->z == v.z && this->w == v.w);
	}

	bool IntVector4::Zero() const noexcept
	{
		return this->x == 0.0f && this->y == 0.0f && this->z == 0.0f && this->w == 0.0f;
	}

	IntVector4 IntVector4::Reset() noexcept
	{
		return IntVector4(0, 0, 0, 0);
	}

	IntVector4 IntVector4::Normalize(IntVector4 vec) noexcept
	{
		vec.x /= (std::int32_t)vec.Length();
		vec.y /= (std::int32_t)vec.Length();
		vec.z /= (std::int32_t)vec.Length();
		vec.w /= (std::int32_t)vec.Length();

		return vec;
	}

	float IntVector4::Length() const noexcept
	{
		return (float)sqrt(x * x + y * y + z * z + w * w);
	}
}