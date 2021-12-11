#include "Vector.h"

namespace S2DE::Math
{
	inline Vector2 Vector2::operator = (const Vector2& v) noexcept
	{
		x = v.x;
		y = v.y;

		return *this;
	}

	inline Vector2 Vector2::operator -= (const Vector2& v) noexcept
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}

	inline Vector2 Vector2::operator += (const Vector2& v) noexcept
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	inline Vector2 Vector2::operator / (const float v) const noexcept
	{
		return  Vector2(x / v, y / v);
	}

	inline Vector2 Vector2::operator / (const Vector2& v) const noexcept
	{
		return  Vector2(x / v.x, y / v.y);
	}

	inline Vector2 Vector2::operator * (const float v) const noexcept
	{
		return  Vector2(x * v, y * v);
	}

	inline Vector2 Vector2::operator * (const Vector2& v) const noexcept
	{
		return  Vector2(x * v.x, y * v.y);
	}

	inline Vector2 Vector2::operator + (const float v) const noexcept
	{
		return  Vector2(x + v, y + v);
	}

	inline Vector2 Vector2::operator - (const float v) const noexcept
	{
		return  Vector2(x - v, y - v);
	}

	inline Vector2 Vector2::operator + () const noexcept
	{
		return *this;
	}

	inline Vector2 Vector2::operator + (const Vector2& v) const noexcept
	{
		return Vector2(x + v.x, y + v.y);
	}

	inline Vector2 Vector2::operator - (const Vector2& v) const noexcept
	{
		return Vector2(x - v.x, y - v.y);
	}

	inline Vector2 Vector2::operator - () const noexcept
	{
		return Vector2(-x, -y);
	}

	inline bool Vector2::operator ==(const Vector2& v) const noexcept
	{
		return (this->x == v.x && this->y == v.y);
	}

	inline bool Vector2::operator !=(const Vector2& v) const noexcept
	{
		return !(this->x == v.x && this->y == v.y);
	}

	inline Vector2 Vector2::Normalize(Vector2 vec) noexcept
	{
		vec.x /= vec.Length();
		vec.y /= vec.Length();

		return vec;
	}

	inline float Vector2::Length() const noexcept
	{
		return sqrt(x*x + y * y);
	}

	inline bool Vector2::Zero() const noexcept
	{
		return this->x == 0.0f && this->y == 0.0f;
	}

	inline Vector3 Vector3::operator = (const Vector3& v)  noexcept
	{
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	inline Vector3 Vector3::operator = (const Vector2& v)  noexcept
	{
		x = v.x;
		y = v.y;
		
		return *this;
	}

	inline Vector3 Vector3::operator -= (const Vector3& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	inline Vector3 Vector3::operator += (const Vector3& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	inline Vector3 Vector3::operator / (const float v) const noexcept
	{
		return  Vector3(x / v, y / v, z / v);
	}

	inline Vector3 Vector3::operator / (const Vector3& v) const noexcept
	{
		return  Vector3(x / v.x, y / v.y, z / v.z);
	}

	inline Vector3 Vector3::operator * (const float v) const noexcept
	{
		return  Vector3(x * v, y * v, z * v);
	}

	inline Vector3 Vector3::operator * (const Vector3& v) const noexcept
	{
		return  Vector3(x * v.x, y * v.y, z * v.z);
	}

	inline Vector3 Vector3::operator + (const float v) const noexcept
	{
		return  Vector3(x + v, y + v, z + v);
	}

	inline Vector3 Vector3::operator - (const float v) const noexcept
	{
		return  Vector3(x - v, y - v, z - v);
	}

	inline Vector3 Vector3::operator + () const noexcept
	{
		return *this;
	}

	inline Vector3 Vector3::operator + (const Vector3& v) const noexcept
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	inline Vector3 Vector3::operator - (const Vector3& v) const noexcept
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	inline Vector3 Vector3::operator - () const noexcept
	{
		return Vector3(-x, -y, -z);
	}

	inline bool Vector3::operator == (const Vector3& v) const noexcept
	{
		return (this->x == v.y && this->y == v.y && this->z == v.z);
	}

	inline bool Vector3::operator !=(const Vector3& v) const noexcept
	{
		return !(this->x == v.x && this->y == v.y && this->z == v.z);
	}

	inline bool Vector3::Zero() const noexcept
	{
		return this->x == 0.0f && this->y == 0.0f&& this->z == 0.0f;
	}

	inline Vector3 Vector3::Normalize(Vector3 vec) noexcept
	{
		vec.x /= vec.Length();
		vec.y /= vec.Length();
		vec.z /= vec.Length();

		return vec;
	}

	inline float Vector3::Length() const noexcept
	{
		return sqrt(x * x + y * y + z * z);
	}

	inline Vector4 Vector4::operator = (const Vector4& v) noexcept
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;

		return *this;
	}

	inline Vector4 Vector4::operator -= (const Vector4& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;

		return *this;
	}

	inline Vector4 Vector4::operator += (const Vector4& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}

	inline Vector4 Vector4::operator / (const float v) const noexcept
	{
		return  Vector4(x / v, y / v, z / v, w / v);
	}

	inline Vector4 Vector4::operator / (const Vector4& v) const noexcept
	{
		return  Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
	}

	inline Vector4 Vector4::operator + (const float v) const noexcept
	{
		return  Vector4(x + v, y + v, z + v, w + v);
	}

	inline Vector4 Vector4::operator - (const float v) const noexcept
	{
		return  Vector4(x - v, y - v, z - v, w - v);
	}

	inline Vector4 Vector4::operator + () const noexcept
	{
		return *this;
	}

	inline Vector4 Vector4::operator * (const float v) const noexcept
	{
		return  Vector4(x * v, y * v, z * v, w * v);
	}

	inline Vector4 Vector4::operator * (const Vector4& v) const noexcept
	{
		return  Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
	}

	inline Vector4 Vector4::operator + (const Vector4& v) const noexcept
	{
		return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	inline Vector4 Vector4::operator - (const Vector4& v) const noexcept
	{
		return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	inline Vector4 Vector4::operator - () const noexcept
	{
		return Vector4(-x, -y, -z, -w);
	}

	inline bool Vector4::operator == (const Vector4& v) const noexcept
	{
		return (this->x == v.y && this->y == v.y && this->z == v.z && this->w == v.w);
	}

	inline bool Vector4::operator !=(const Vector4 & v) const noexcept
	{
		return !(this->x == v.y && this->y == v.y && this->z == v.z && this->w == v.w);
	}

	inline bool Vector4::Zero() const noexcept
	{
		return this->x == 0.0f && this->y == 0.0f && this->z == 0.0f && this->w == 0.0f;
	}

	inline Vector4 Vector4::Normalize(Vector4 vec) noexcept
	{
		vec.x /= vec.Length();
		vec.y /= vec.Length();
		vec.z /= vec.Length();
		vec.w /= vec.Length();

		return vec;
	}

	inline float Vector4::Length() const noexcept
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}
}