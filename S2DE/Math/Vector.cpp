#include "Vector.h"

namespace S2DE
{
	Vector2 Vector2::operator = (const Vector2& v) noexcept
	{
		x = v.x;
		y = v.y;

		return *this;
	}

	Vector2 Vector2::operator -= (const Vector2& v) noexcept
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}

	Vector2 Vector2::operator += (const Vector2& v) noexcept
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	Vector2 Vector2::operator / (const float v) const noexcept
	{
		return  Vector2(x / v, y / v);
	}

	Vector2 Vector2::operator / (const Vector2& v) const noexcept
	{
		return  Vector2(x / v.x, y / v.y);
	}

	Vector2 Vector2::operator * (const float v) const noexcept
	{
		return  Vector2(x * v, y * v);
	}

	Vector2 Vector2::operator * (const Vector2& v) const noexcept
	{
		return  Vector2(x * v.x, y * v.y);
	}

	Vector2 Vector2::operator + (const float v) const noexcept
	{
		return  Vector2(x + v, y + v);
	}

	Vector2 Vector2::operator - (const float v) const noexcept
	{
		return  Vector2(x - v, y - v);
	}

	Vector2 Vector2::operator + () const noexcept
	{
		return *this;
	}

	Vector2 Vector2::operator + (const Vector2& v) const noexcept
	{
		return Vector2(x + v.x, y + v.y);
	}

	Vector2 Vector2::operator - (const Vector2& v) const noexcept
	{
		return Vector2(x - v.x, y - v.y);
	}

	Vector2 Vector2::Reset() noexcept
	{
		return Vector2(0.0f, 0.0f);
	}

	Vector2 Vector2::operator - () const noexcept
	{
		return Vector2(-x, -y);
	}

	bool Vector2::operator ==(const Vector2& v) const noexcept
	{
		return (this->x == v.x && this->y == v.y);
	}

	bool Vector2::operator !=(const Vector2& v) const noexcept
	{
		return !(this->x == v.x && this->y == v.y);
	}

	Vector2 Vector2::Normalize(Vector2 vec) noexcept
	{
		vec.x /= vec.Length();
		vec.y /= vec.Length();

		return vec;
	}

	float Vector2::Length() const noexcept
	{
		return sqrt(x*x + y * y);
	}

	bool Vector2::Zero() const noexcept
	{
		return this->x == 0.0f && this->y == 0.0f;
	}

	Vector3 Vector3::operator = (const Vector3& v)  noexcept
	{
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	Vector3 Vector3::operator = (const Vector2& v)  noexcept
	{
		x = v.x;
		y = v.y;
		
		return *this;
	}

	Vector3 Vector3::operator -= (const Vector3& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	Vector3 Vector3::operator += (const Vector3& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	Vector3 Vector3::operator / (const float v) const noexcept
	{
		return  Vector3(x / v, y / v, z / v);
	}

	Vector3 Vector3::operator / (const Vector3& v) const noexcept
	{
		return  Vector3(x / v.x, y / v.y, z / v.z);
	}

	Vector3 Vector3::operator * (const float v) const noexcept
	{
		return  Vector3(x * v, y * v, z * v);
	}

	Vector3 Vector3::operator * (const Vector3& v) const noexcept
	{
		return  Vector3(x * v.x, y * v.y, z * v.z);
	}

	Vector3 Vector3::operator + (const float v) const noexcept
	{
		return  Vector3(x + v, y + v, z + v);
	}

	Vector3 Vector3::operator - (const float v) const noexcept
	{
		return  Vector3(x - v, y - v, z - v);
	}

	Vector3 Vector3::operator + () const noexcept
	{
		return *this;
	}

	Vector3 Vector3::operator + (const Vector3& v) const noexcept
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 Vector3::operator - (const Vector3& v) const noexcept
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 Vector3::operator - () const noexcept
	{
		return Vector3(-x, -y, -z);
	}

	bool Vector3::operator == (const Vector3& v) const noexcept
	{
		return (this->x == v.y && this->y == v.y && this->z == v.z);
	}

	bool Vector3::operator !=(const Vector3& v) const noexcept
	{
		return !(this->x == v.x && this->y == v.y && this->z == v.z);
	}

	bool Vector3::Zero() const noexcept
	{
		return this->x == 0.0f && this->y == 0.0f&& this->z == 0.0f;
	}

	Vector3 Vector3::Reset() noexcept
	{
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	Vector3 Vector3::Normalize(Vector3 vec) noexcept
	{
		vec.x /= vec.Length();
		vec.y /= vec.Length();
		vec.z /= vec.Length();

		return vec;
	}

	float Vector3::Length() const noexcept
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vector4 Vector4::operator = (const Vector4& v) noexcept
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;

		return *this;
	}

	Vector4 Vector4::operator -= (const Vector4& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;

		return *this;
	}

	Vector4 Vector4::operator += (const Vector4& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}

	Vector4 Vector4::operator / (const float v) const noexcept
	{
		return  Vector4(x / v, y / v, z / v, w / v);
	}

	Vector4 Vector4::operator / (const Vector4& v) const noexcept
	{
		return  Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
	}

	Vector4 Vector4::operator + (const float v) const noexcept
	{
		return  Vector4(x + v, y + v, z + v, w + v);
	}

	Vector4 Vector4::operator - (const float v) const noexcept
	{
		return  Vector4(x - v, y - v, z - v, w - v);
	}

	Vector4 Vector4::operator + () const noexcept
	{
		return *this;
	}

	Vector4 Vector4::operator * (const float v) const noexcept
	{
		return  Vector4(x * v, y * v, z * v, w * v);
	}

	Vector4 Vector4::operator * (const Vector4& v) const noexcept
	{
		return  Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
	}

	Vector4 Vector4::operator + (const Vector4& v) const noexcept
	{
		return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Vector4 Vector4::operator - (const Vector4& v) const noexcept
	{
		return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	Vector4 Vector4::operator - () const noexcept
	{
		return Vector4(-x, -y, -z, -w);
	}

	bool Vector4::operator == (const Vector4& v) const noexcept
	{
		return (this->x == v.y && this->y == v.y && this->z == v.z && this->w == v.w);
	}

	bool Vector4::operator !=(const Vector4 & v) const noexcept
	{
		return !(this->x == v.y && this->y == v.y && this->z == v.z && this->w == v.w);
	}

	bool Vector4::Zero() const noexcept
	{
		return this->x == 0.0f && this->y == 0.0f && this->z == 0.0f && this->w == 0.0f;
	}

	Vector4 Vector4::Reset() noexcept
	{
		return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	Vector4 Vector4::Normalize(Vector4 vec) noexcept
	{
		vec.x /= vec.Length();
		vec.y /= vec.Length();
		vec.z /= vec.Length();
		vec.w /= vec.Length();

		return vec;
	}

	float Vector4::Length() const noexcept
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}
}