#pragma once
#include "Base/Main/Common.h"

//////////////////////////////////////////////
//Boost includes
//////////////////////////////////////////////
#include <boost/random.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/random_device.hpp>

namespace S2DE
{
	template<class T>
	static T RandomRange(T range)
	{
		boost::random::random_device rngdevice;
		boost::random::mt19937 gen(rngdevice);
		if (range == static_cast<float>(range) || range == static_cast<double>(range))
		{
			boost::random::uniform_real_distribution<> dist_real{ 0.0, (double)range };
			return static_cast<T>(dist_real(gen));
		}
		else
		{
			boost::random::uniform_int_distribution<> dist_int{ 0, (std::int32_t)range };
			return static_cast<T>(dist_int(gen));
		}


		return 0;
	}

	static bool RandomBool()
	{
		boost::random::random_device rngdevice;
		boost::random::mt19937 gen(rngdevice);
		boost::random::bernoulli_distribution<> dist;

		return static_cast<bool>(dist(gen));
	}

	template<class T>
	static T Random()
	{
		boost::random::random_device gen;
		return  static_cast<T>(gen());
	}

	template<class T>
	static T RandomByMinMax(T min, T max)
	{
		boost::random::random_device rngdevice;
		boost::random::mt19937 gen(rngdevice);

		if (max == static_cast<float>(max) || max == static_cast<double>(max))
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

}



