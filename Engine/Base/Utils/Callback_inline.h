#pragma once
#include "Base/Utils/Callback.h"

namespace S2DE::Core::Utils
{
	template<typename T>
	Callback<T>::Callback()
		: m_storage()
	{

	}

	template<typename T>
	Callback<T>::Callback(const T& callback)
		: m_storage()
	{
		AddCallback(callback);
	}

	template<typename T>
	Callback<T>::~Callback()
	{
		m_storage.clear();
	}

	template<typename T>
	void Callback<T>::UpdateCallback(const T& callback)
	{
		RemoveCallback(callback);
		AddCallback(callback);
	}

	template<typename T>
	void Callback<T>::AddCallback(const T& callback)
	{
		m_storage.push_back(callback);
	}

	template<typename T>
	void Callback<T>::RemoveCallback(const T& callback)
	{
		m_storage.erase(std::remove(m_storage.begin(), m_storage.end(), callback), m_storage.end());
	}

	template<typename T>
	void Callback<T>::Clear()
	{
		m_storage.clear();
	}

	template<typename T>
	void Callback<T>::RunAllCallbacks()
	{
		for (const auto& callback : m_storage)
		{
			if (callback != nullptr)
			{
				callback();
			}
		}
	}
}
