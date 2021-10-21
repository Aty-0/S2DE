#pragma once
#include "Base/Main/Common.h"


namespace S2DE
{
	class S2DE_API IntVector2
	{
	public:
		IntVector2() :
			x(0),
			y(0)
		{

		}

		IntVector2(std::int32_t _x, std::int32_t _y) :
			x(_x),
			y(_y)
		{

		}

		IntVector2(std::nullptr_t) :
			x(0),
			y(0)
		{

		}

		inline IntVector2		   operator = (const IntVector2& v) noexcept;
		inline IntVector2		   operator -= (const IntVector2& v) noexcept;
		inline IntVector2		   operator += (const IntVector2& v) noexcept;

		inline IntVector2		   operator / (const std::int32_t v) const noexcept;
		inline IntVector2		   operator / (const IntVector2& v) const noexcept;

		inline IntVector2		   operator * (const std::int32_t v) const noexcept;
		inline IntVector2		   operator * (const IntVector2& v) const noexcept;

		inline IntVector2		   operator + (const std::int32_t v) const noexcept;
		inline IntVector2		   operator + () const noexcept;
		inline IntVector2		   operator + (const IntVector2& v) const noexcept;

		inline IntVector2		   operator - (const std::int32_t v) const noexcept;
		inline IntVector2		   operator - (const IntVector2& v) const noexcept;
		inline IntVector2		   operator - () const noexcept;

		inline bool		   operator ==(const IntVector2& v) const noexcept;
		inline bool		   operator !=(const IntVector2& v) const noexcept;

		inline bool					Zero() const noexcept;
		static inline IntVector2	Reset() noexcept;
		static inline IntVector2	Normalize(IntVector2 vec) noexcept;
		inline float				Length() const noexcept;


		std::int32_t x;
		std::int32_t y;
	};


	class S2DE_API IntVector3
	{
	public:
		IntVector3() :
			x(0),
			y(0),
			z(0)
		{

		}

		IntVector3(std::int32_t _x, std::int32_t _y) :
			x(_x),
			y(_y),
			z(0)
		{

		}
		IntVector3(std::int32_t _x, std::int32_t _y, std::int32_t _z) :
			x(_x),
			y(_y),
			z(_z)
		{

		}

		IntVector3(IntVector2 v) :
			x(v.x),
			y(v.y),
			z(0)
		{

		}

		IntVector3(std::nullptr_t) :
			x(0),
			y(0),
			z(0)
		{

		}

		inline IntVector3			operator = (const IntVector3& v)  noexcept;
		inline IntVector3			operator = (const IntVector2& v)  noexcept;
		inline IntVector3			operator -= (const IntVector3& v) noexcept;
		inline IntVector3			operator += (const IntVector3& v) noexcept;

		inline IntVector3			operator / (const std::int32_t v) const noexcept;
		inline IntVector3			operator / (const IntVector3& v) const noexcept;

		inline IntVector3			operator * (const std::int32_t v) const noexcept;
		inline IntVector3			operator * (const IntVector3& v) const noexcept;

		inline IntVector3			operator + (const std::int32_t v) const noexcept;
		inline IntVector3			operator + () const noexcept;
		inline IntVector3			operator + (const IntVector3& v) const noexcept;

		inline IntVector3			operator - (const std::int32_t v) const noexcept;
		inline IntVector3			operator - (const IntVector3& v) const noexcept;
		inline IntVector3			operator - () const noexcept;

		inline bool					operator == (const IntVector3& v) const noexcept;
		inline bool					operator !=(const IntVector3& v) const noexcept;

		inline bool					Zero() const noexcept;
		static inline IntVector3	Reset() noexcept;
		static inline IntVector3	Normalize(IntVector3 vec) noexcept;
		inline float				Length() const noexcept;

		std::int32_t x;
		std::int32_t y;
		std::int32_t z;
	};

	class S2DE_API IntVector4
	{
	public:
		IntVector4() :
			x(0),
			y(0),
			z(0),
			w(0)
		{

		}

		IntVector4(std::int32_t _x, std::int32_t _y) :
			x(_x),
			y(_y),
			z(0),
			w(0)
		{

		}

		IntVector4(std::int32_t _x, std::int32_t _y, std::int32_t _z) :
			x(_x),
			y(_y),
			z(_z),
			w(0)
		{

		}

		IntVector4(std::int32_t _x, std::int32_t _y, std::int32_t _z, std::int32_t _w) :
			x(_x),
			y(_y),
			z(_z),
			w(_w)
		{

		}

		IntVector4(IntVector2 v) :
			x(v.x),
			y(v.y),
			z(0),
			w(0)
		{

		}

		IntVector4(IntVector3 v) :
			x(v.x),
			y(v.y),
			z(v.z),
			w(0)
		{

		}

		IntVector4(std::nullptr_t) :
			x(0),
			y(0),
			z(0),
			w(0)
		{

		}


		inline IntVector4				operator = (const IntVector4& v)  noexcept;
		inline IntVector4				operator -= (const IntVector4& v) noexcept;
		inline IntVector4				operator += (const IntVector4& v) noexcept;

		inline IntVector4				operator / (const std::int32_t v) const noexcept;
		inline IntVector4				operator / (const IntVector4& v) const noexcept;

		inline IntVector4				operator * (const std::int32_t v) const noexcept;
		inline IntVector4				operator * (const IntVector4& v) const noexcept;

		inline IntVector4				operator + () const noexcept;
		inline IntVector4				operator + (const std::int32_t v) const noexcept;
		inline IntVector4				operator + (const IntVector4& v) const noexcept;

		inline IntVector4				operator - (const IntVector4& v) const noexcept;
		inline IntVector4				operator - (const std::int32_t v) const noexcept;
		inline IntVector4				operator - () const noexcept;

		inline bool					operator == (const IntVector4& v) const noexcept;
		inline bool					operator !=(const IntVector4& v) const noexcept;

		inline bool					Zero() const noexcept;
		static inline IntVector4	Reset() noexcept;
		static inline IntVector4	Normalize(IntVector4 vec) noexcept;
		inline float				Length() const noexcept;

		std::int32_t x;
		std::int32_t y;
		std::int32_t z;
		std::int32_t w;
	};
}