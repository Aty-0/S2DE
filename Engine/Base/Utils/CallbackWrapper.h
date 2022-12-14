#pragma once
#include "Base/Main/Common.h"

namespace S2DE::Core::Utils
{
	// TODO: 1. Make run, remove callback by string
	//									  by function ref
	//		 2. Rename ?

	typedef std::function<void()> DefaultCallback;

	template<typename T = DefaultCallback>
	class CallbackWrapper
	{
	public:
		CallbackWrapper()
			: m_storage()
		{
			
		}

		~CallbackWrapper()
		{
			m_storage.clear();
		}

		void UpdateCallback(const T& callback)
		{
			RemoveCallback(callback);
			AddCallback(callback);
		}

		void AddCallback(const T& callback)
		{
			m_storage.push_back(callback);
		}

		void RemoveCallback(const T& callback)
		{
			m_storage.erase(std::remove(m_storage.begin(), m_storage.end(), callback), m_storage.end());
		}

		void Clear()
		{
			m_storage.clear();
		}

		void RunAllCallbacks()
		{
			for(const auto& callback : m_storage)
			{
				if (callback != nullptr)
				{
					callback();
				}
			}
		}

	private:
		std::vector<T> m_storage;
	};
}
