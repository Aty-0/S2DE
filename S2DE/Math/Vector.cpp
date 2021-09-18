#include "Vector.h"

namespace S2DE
{
	Vector2::Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

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

	Vector2 Vector2::Zero()
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

	void Vector2::Normalize()
	{
		x /= Length();
		y /= Length();
	}

	float Vector2::Length()
	{
		return sqrt(x*x + y * y);
	}

	bool Vector2::ZeroCordinate() const noexcept
	{
		return this->x == 0.0f && this->y == 0.0f;
	}

	Vector3::Vector3(Vector2 v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = 0.0f;
	}

	Vector3::Vector3(float x, float y)
	{
		this->x = x;
		this->y = y;
		this->z = 0.0f;
	}

	Vector3::Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
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

	bool Vector3::ZeroCordinate() const noexcept
	{
		return this->x == 0.0f && this->y == 0.0f&& this->z == 0.0f;
	}

	Vector3 Vector3::Zero()
	{
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	void Vector3::Normalize()
	{
		x /= Length();
		y /= Length();
		z /= Length();
	}

	float Vector3::Length()
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vector4::Vector4(float x, float  y, float  z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
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

	bool Vector4::ZeroCordinate() const noexcept
	{
		return this->x == 0.0f && this->y == 0.0f && this->z == 0.0f && this->w == 0.0f;
	}

	Vector4 Vector4::Zero()
	{
		return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	void Vector4::Normalize()
	{
		x /= Length();
		y /= Length();
		z /= Length();
		w /= Length();
	}

	float Vector4::Length()
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}
}