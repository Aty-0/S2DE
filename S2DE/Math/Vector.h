#pragma once
#include "Base/Main/Common.h"


namespace S2DE
{
	class S2DE_API Vector2
	{
	public:
		Vector2() = default;
		Vector2(float x, float y);
		Vector2(std::nullptr_t n) { x = 0; y = 0; }

		Vector2		   operator = (const Vector2& v) noexcept;
		Vector2		   operator -= (const Vector2& v) noexcept;
		Vector2		   operator += (const Vector2& v) noexcept;

		Vector2		   operator / (const float v) const noexcept;
		Vector2		   operator / (const Vector2& v) const noexcept;

		Vector2		   operator * (const float v) const noexcept;
		Vector2		   operator * (const Vector2& v) const noexcept;

		Vector2		   operator + (const float v) const noexcept;
		Vector2		   operator + () const noexcept;
		Vector2		   operator + (const Vector2& v) const noexcept;

		Vector2		   operator - (const float v) const noexcept;
		Vector2		   operator - (const Vector2& v) const noexcept;
		Vector2		   operator - () const noexcept;

		bool		   operator ==(const Vector2& v) const noexcept;
		bool		   operator !=(const Vector2& v) const noexcept;

		bool		   ZeroCordinate() const noexcept;
		static Vector2 Zero();
		void           Normalize();
		float          Length();


		float x = 0.0f;
		float y = 0.0f;
	};


	class S2DE_API Vector3
	{
	public:
		Vector3() = default;
		Vector3(float x, float y, float z);
		Vector3(float x, float y);
		Vector3(Vector2 v);
		Vector3(std::nullptr_t n) { x = 0; y = 0; z = 0; }

		Vector3			operator = (const Vector3& v)  noexcept;
		Vector3			operator = (const Vector2& v)  noexcept;
		Vector3			operator -= (const Vector3& v) noexcept;
		Vector3			operator += (const Vector3& v) noexcept;

		Vector3			operator / (const float v) const noexcept;
		Vector3			operator / (const Vector3& v) const noexcept;

		Vector3			operator * (const float v) const noexcept;
		Vector3			operator * (const Vector3& v) const noexcept;

		Vector3			operator + (const float v) const noexcept;
		Vector3			operator + () const noexcept;
		Vector3			operator + (const Vector3& v) const noexcept;

		Vector3			operator - (const float v) const noexcept;
		Vector3			operator - (const Vector3& v) const noexcept;
		Vector3			operator - () const noexcept;

		bool			operator == (const Vector3& v) const noexcept;
		bool			operator !=(const Vector3& v) const noexcept;

		bool			ZeroCordinate() const noexcept;
		static	Vector3 Zero();
		void			Normalize();
		float			Length();

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};

	class S2DE_API Vector4
	{
	public:
		Vector4() = default;
		Vector4(float x, float y, float z, float w);
		Vector4(std::nullptr_t n) { x = 0;  y = 0; z = 0; w = 0; }

		Vector4			operator = (const Vector4& v)  noexcept;
		Vector4			operator -= (const Vector4& v) noexcept;
		Vector4			operator += (const Vector4& v) noexcept;

		Vector4			operator / (const float v) const noexcept;
		Vector4			operator / (const Vector4& v) const noexcept;

		Vector4			operator * (const float v) const noexcept;
		Vector4			operator * (const Vector4& v) const noexcept;

		Vector4			operator + () const noexcept;
		Vector4			operator + (const float v) const noexcept;
		Vector4			operator + (const Vector4& v) const noexcept;

		Vector4			operator - (const Vector4& v) const noexcept;
		Vector4			operator - (const float v) const noexcept;
		Vector4			operator - () const noexcept;

		bool			operator == (const Vector4& v) const noexcept;
		bool			operator !=(const Vector4 & v) const noexcept;

		bool			ZeroCordinate() const noexcept;
		static	Vector4 Zero();
		void			Normalize();
		float			Length();

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;
	};

	inline static XVector To_XMVector4(Vector4 v)
	{
		XVector xv = DirectX::XMVectorSet(v.x, v.y, v.z, v.w);
		return DirectX::XMVector4Dot(xv, xv);
	}

	inline static XVector To_XMVector3(Vector3 v)
	{
		XVector xv = DirectX::XMVectorSet(v.x, v.y, v.z, 0);
		return DirectX::XMVector3Dot(xv, xv);
	}

	inline static XVector To_XMVector2(Vector2 v)
	{
		XVector xv = DirectX::XMVectorSet(v.x, v.y, 0, 0);
		return DirectX::XMVector2Dot(xv, xv);
	}
}