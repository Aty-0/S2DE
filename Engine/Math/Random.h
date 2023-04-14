#pragma once
#include <boost/random.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/random_device.hpp>

namespace S2DE::Math
{
	class Random
	{
	public:
		[[nodiscard]] static bool RandomBool()
		{
			boost::random::random_device rngdevice;
			boost::random::mt19937 gen(rngdevice);
			boost::random::bernoulli_distribution<> dist;

			return static_cast<bool>(dist(gen));
		}

		template<class T>
		[[nodiscard]] static T RandomNum()
		{
			boost::random::random_device gen;
			return  static_cast<T>(gen());
		}

		template<class T>
		[[nodiscard]] static T RandomRange(T max)
		{
			boost::random::random_device rngdevice;
			boost::random::mt19937 gen(rngdevice);

			if (std::is_same<T, float>::value)
			{
				boost::random::uniform_real_distribution<> dist_real{ 0.0f, static_cast<float>(max) };
				return static_cast<T>(dist_real(gen));
			}
			else
			{
				boost::random::uniform_int_distribution<> dist_int{ 0, static_cast<std::int32_t>(max) };
				return static_cast<T>(dist_int(gen));
			}


			return 0;
		}

		template<class T>
		[[nodiscard]] static T RandomRange(T min, T max)
		{
			boost::random::random_device rngdevice;
			boost::random::mt19937 gen(rngdevice);

			if (std::is_same<T, float>::value)
			{
				boost::random::uniform_real_distribution<> dist_real{ static_cast<float>(min), static_cast<float>(max) };
				return static_cast<T>(dist_real(gen));
			}
			else
			{
				boost::random::uniform_int_distribution<> dist_int{ static_cast<std::int32_t>(min), static_cast<std::int32_t>(max) };
				return static_cast<T>(dist_int(gen));
			}


			return 0;
		}
	};
}



