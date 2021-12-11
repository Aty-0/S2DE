#pragma once
#include "Base/Main/Common.h"


namespace S2DE::Math
{
	class S2DE_API Vector2
	{
	public:
		Vector2() :
			x(0.0f),
			y(0.0f)
		{

		}

		Vector2(float _x, float _y) :
			x(_x),
			y(_y)
		{

		}

		Vector2(std::nullptr_t) : 
			x(0.0f),
			y(0.0f)
		{ 

		}

		inline Vector2		   operator = (const Vector2& v) noexcept;
		inline Vector2		   operator -= (const Vector2& v) noexcept;
		inline Vector2		   operator += (const Vector2& v) noexcept;

		inline Vector2		   operator / (const float v) const noexcept;
		inline Vector2		   operator / (const Vector2& v) const noexcept;

		inline Vector2		   operator * (const float v) const noexcept;
		inline Vector2		   operator * (const Vector2& v) const noexcept;

		inline Vector2		   operator + (const float v) const noexcept;
		inline Vector2		   operator + () const noexcept;
		inline Vector2		   operator + (const Vector2& v) const noexcept;

		inline Vector2		   operator - (const float v) const noexcept;
		inline Vector2		   operator - (const Vector2& v) const noexcept;
		inline Vector2		   operator - () const noexcept;

		inline bool		   operator ==(const Vector2& v) const noexcept;
		inline bool		   operator !=(const Vector2& v) const noexcept;

		inline bool					Zero() const noexcept;
		static inline Vector2		Reset() noexcept { return Vector2(0.0f, 0.0f); }
		static inline Vector2		Normalize(Vector2 vec) noexcept;
		inline float				Length() const noexcept;


		float x;
		float y;
	};


	class S2DE_API Vector3
	{
	public:
		Vector3() :
			x(0.0f),
			y(0.0f),
			z(0.0f)
		{

		}

		Vector3(float _x, float _y) :
			x(_x),
			y(_y),
			z(0.0f)
		{

		}
		Vector3(float _x, float _y, float _z) :
			x(_x),
			y(_y),
			z(_z)
		{

		}
		Vector3(Vector2 v) :
			x(v.x),
			y(v.y),
			z(0.0f)
		{

		}

		Vector3(std::nullptr_t) :
			x(0.0f),
			y(0.0f),
			z(0.0f)
		{

		}

		inline Vector3			operator = (const Vector3& v)  noexcept;
		inline Vector3			operator = (const Vector2& v)  noexcept;
		inline Vector3			operator -= (const Vector3& v) noexcept;
		inline Vector3			operator += (const Vector3& v) noexcept;

		inline Vector3			operator / (const float v) const noexcept;
		inline Vector3			operator / (const Vector3& v) const noexcept;

		inline Vector3			operator * (const float v) const noexcept;
		inline Vector3			operator * (const Vector3& v) const noexcept;

		inline Vector3			operator + (const float v) const noexcept;
		inline Vector3			operator + () const noexcept;
		inline Vector3			operator + (const Vector3& v) const noexcept;

		inline Vector3			operator - (const float v) const noexcept;
		inline Vector3			operator - (const Vector3& v) const noexcept;
		inline Vector3			operator - () const noexcept;

		inline bool					operator == (const Vector3& v) const noexcept;
		inline bool					operator !=(const Vector3& v) const noexcept;

		inline bool					Zero() const noexcept;
		static inline Vector3		Reset() noexcept { return Vector3(0.0f, 0.0f, 0.0f); }
		static inline Vector3		Normalize(Vector3 vec) noexcept;
		inline float				Length() const noexcept;

		float x;
		float y;
		float z;
	};

	class S2DE_API Vector4
	{
	public:
		Vector4() :
			x(0.0f),
			y(0.0f),
			z(0.0f),
			w(0.0f)
		{

		}

		Vector4(float _x, float _y) :
			x(_x),
			y(_y),
			z(0.0f),
			w(0.0f)
		{

		}

		Vector4(float _x, float _y, float _z) :
			x(_x),
			y(_y),
			z(_z),
			w(0.0f)
		{

		}

		Vector4(float _x, float _y, float _z, float _w) :
			x(_x),
			y(_y),
			z(_z),
			w(_w)
		{

		}

		Vector4(Vector2 v) :
			x(v.x),
			y(v.y),
			z(0.0f),
			w(0.0f)
		{

		}

		Vector4(Vector3 v) :
			x(v.x),
			y(v.y),
			z(v.z),
			w(0.0f)
		{

		}

		Vector4(std::nullptr_t) :
			x(0.0f),
			y(0.0f),
			z(0.0f),
			w(0.0f)
		{

		}
	

		inline Vector4				operator = (const Vector4& v)  noexcept;
		inline Vector4				operator -= (const Vector4& v) noexcept;
		inline Vector4				operator += (const Vector4& v) noexcept;

		inline Vector4				operator / (const float v) const noexcept;
		inline Vector4				operator / (const Vector4& v) const noexcept;

		inline Vector4				operator * (const float v) const noexcept;
		inline Vector4				operator * (const Vector4& v) const noexcept;

		inline Vector4				operator + () const noexcept;
		inline Vector4				operator + (const float v) const noexcept;
		inline Vector4				operator + (const Vector4& v) const noexcept;

		inline Vector4				operator - (const Vector4& v) const noexcept;
		inline Vector4				operator - (const float v) const noexcept;
		inline Vector4				operator - () const noexcept;

		inline bool					operator == (const Vector4& v) const noexcept;
		inline bool					operator !=(const Vector4 & v) const noexcept;

		inline bool					Zero() const noexcept;
		static inline Vector4		Reset() noexcept { return Vector4(0.0f, 0.0f, 0.0f, 0.0f); }
		static inline Vector4		Normalize(Vector4 vec) noexcept;
		inline float				Length() const noexcept;

		float x;
		float y;
		float z;
		float w;
	};

	inline static XVector To_XMVector4(Vector4 v)
	{
		return DirectX::XMVectorSet(v.x, v.y, v.z, v.w);
	}

	inline static XVector To_XMVector3(Vector3 v)
	{
		return DirectX::XMVectorSet(v.x, v.y, v.z, 0);
	}

	inline static XVector To_XMVector2(Vector2 v)
	{
		return DirectX::XMVectorSet(v.x, v.y, 0, 0);
	}
}