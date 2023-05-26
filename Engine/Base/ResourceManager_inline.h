#pragma once
#include "Base/ResourceManager.h"

namespace S2DE::Core::Resources
{
	template <typename T>
	inline T* ResourceManager::Create(std::string name, bool cantDelete)
	{
		if (Core::Utils::isStringEmpty(name))
		{
			Core::Utils::Logger::Error("[ResourceManager] Resource name is empty!");
			return nullptr;
		}

		auto resource = new T();
		resource->m_cantDelete = cantDelete;
		auto key = std::make_pair(Core::Utils::isStringEmpty(name) ? resource->m_name : name, std::type_index(typeid(T)));

		return (m_storage[key] = resource); // Return resource pointer
	}

	template <typename T>
	inline bool ResourceManager::Add(T* resource, std::string name, bool cantDelete)
	{
		if (resource == nullptr)
		{
			Core::Utils::Logger::Error("[ResourceManager] Resource is null!");
			return false;
		}

		// Check resource name 
		// If we have invalid both vars, in resource and custom name
		if (Core::Utils::isStringEmpty(resource->m_name) && Core::Utils::isStringEmpty(name))
		{
			Core::Utils::Logger::Error("[ResourceManager] Resource name is empty!");
			return false;
		}
		else if (Core::Utils::isStringEmpty(resource->m_name))
		{
			resource->m_name = name;
		}

		resource->m_cantDelete = cantDelete;
		auto key = std::make_pair(Core::Utils::isStringEmpty(name) ? resource->m_name : name, std::type_index(typeid(T)));
		m_storage[key] = resource;

		return true;
	}

	template <typename T>
	inline bool ResourceManager::Load(std::string filename, std::string name, bool cantDelete)
	{
		if (Core::Utils::isStringEmpty(filename))
			return false;

		auto resource = new T();

		resource->m_name = Core::Utils::isStringEmpty(name) ? filename : name;
		resource->m_cantDelete = cantDelete;

		if (!resource->Load(filename))
		{
			Delete(resource);
			return false;
		}

		auto key = std::make_pair(Core::Utils::isStringEmpty(name) ? filename : name, std::type_index(typeid(T)));
		m_storage[key] = resource;

		return true;
	}

	template <typename T>
	inline void ResourceManager::Erase(std::string name)
	{
		auto key = std::make_pair(name, std::type_index(typeid(T)));
		auto it = m_storage.find(key);
		if (it != m_storage.end())
		{
			if (it->second->m_cantDelete == false)
			{
				Core::Utils::Logger::Log("[ResourceManager] Erase %s %s", typeid(T).name(), name.c_str());
				m_storage.erase(key);
			}
		}
	}

	template <typename T>
	inline [[nodiscard]] T* ResourceManager::Get(std::string name)
	{
		auto key = std::make_pair(name, std::type_index(typeid(T)));
		auto it = m_storage.find(key);

		if (it == m_storage.end())
		{
			// If we are failed on getting default resource
			if (name == DefaultResourceName)
			{
				Core::Utils::Logger::Error("No default resource loaded for this type of resource!");
				return nullptr;
			}

			// If we are failed on get our resource  
			return Get<T>(DefaultResourceName);
		}

		return reinterpret_cast<T*>(it->second);
	}

	template <typename T>
	inline [[nodiscard]] bool ResourceManager::IsExists(std::string name) const
	{
		auto key = std::make_pair(name, std::type_index(typeid(T)));
		auto it = m_storage.find(key);

		return it != m_storage.end();
	}

	template<typename T>
	inline [[nodiscard]] std::int32_t ResourceManager::GetResourceCount() const
	{
		std::int32_t count = 0;

		for (const auto p : m_storage)
		{
			if (p.first.second == std::type_index(typeid(T)))
			{
				count++;
			}
		}

		return count;
	}
}