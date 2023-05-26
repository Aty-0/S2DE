#pragma once
#include <memory>

namespace S2DE::Core::Utils
{
	template<typename T>
	class Singleton 
	{
	public:
		Singleton() { }
		Singleton(const Singleton&) = delete;
		Singleton(Singleton&&) = delete;
		~Singleton() { /* we can debug it if needed */ }
		Singleton& operator=(const Singleton&) = delete;
		Singleton& operator=(Singleton&&) = delete;

		[[nodiscard]] inline static T* GetInstance() 
		{
			static auto instance = std::make_shared<T>();	
			return instance.get();
		}

	};
}