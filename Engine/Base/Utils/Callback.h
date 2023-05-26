#pragma once
#include "Base/Main/Common.h"

namespace S2DE::Core::Utils
{
	// TODO: Make run, remove callback by string
	
	using DefaultCallbackFunction = std::function<void()>;

	template<typename T = DefaultCallbackFunction>
	class Callback
	{
	public:
		Callback();
		explicit Callback(const T& callback);
		~Callback();

		void UpdateCallback(const T& callback);
		void AddCallback(const T& callback);
		void RemoveCallback(const T& callback);
		void Clear();
		void RunAllCallbacks();

	private:
		std::vector<T> m_storage;
	};
}

#include "Base/Utils/Callback_inline.h"
