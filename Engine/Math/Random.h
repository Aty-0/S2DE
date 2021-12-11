#pragma once
#include "Base/Main/Common.h"

//////////////////////////////////////////////
//Boost includes
//////////////////////////////////////////////
#include <boost/random.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/random_device.hpp>

namespace S2DE::Math
{
	class Random
	{
	public:
		static inline bool RandomBool()
		{
			boost::random::random_device rngdevice;
			boost::random::mt19937 gen(rngdevice);
			boost::random::bernoulli_distribution<> dist;

			return static_cast<bool>(dist(gen));
		}

		template<class T>
		static inline T RandomNum()
		{
			boost::random::random_device gen;
			return  static_cast<T>(gen());
		}

		template<class T>
		static inline T RandomRange(T max)
		{
			boost::random::random_device rngdevice;
			boost::random::mt19937 gen(rngdevice);

			if (std::is_same<T, float>::value)
			{
				boost::random::uniform_real_distribution<> dist_real{ (double)0, (double)max };
				return static_cast<T>(dist_real(gen));
			}
			else
			{
				boost::random::uniform_int_distribution<> dist_int{ (std::int32_t)0, (std::int32_t)max };
				return static_cast<T>(dist_int(gen));
			}


			return 0;
		}

		template<class T>
		static inline T RandomRange(T min, T max)
		{
			boost::random::random_device rngdevice;
			boost::random::mt19937 gen(rngdevice);

			if (std::is_same<T, float>::value)
			{
				boost::random::uniform_real_distribution<> dist_real{ (double)min, (double)max };
				return static_cast<T>(dist_real(gen));
			}
			else
			{
				boost::random::uniform_int_distribution<> dist_int{ (std::int32_t)min, (std::int32_t)max };
				return static_cast<T>(dist_int(gen));
			}


			return 0;
		}
	};
}



