#pragma once
#include "Base/Main/Common.h"
#include "Base/Engine.h"
#include "Base/Resource.h"

#define S2DE_DEFAULT_RESOURCE_DATA_NAME "Gamedata/"
#define S2DE_DEFAULT_FONT_NAME "default"
namespace S2DE::Render
{
	class Texture;
}

namespace S2DE::Core::Resources
{	
	class S2DE_API ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();

		// Clear all resources
		void ClearAll();

		// Get data folder name
		[[nodiscard]] inline std::string	GetNameOfDataFolder() const;


		[[nodiscard]] std::string			GetFilePath(std::string filename, Resource* file);


		bool				GetFilePath(std::string filename, Resource* file, std::string& resultpath);
		bool				GetFilePath(std::string filename, std::string type, std::string ex[], std::string& resultpath);
		bool				GetFilePath(std::string filename, std::string type, std::string ex, std::string& resultpath);

		// Reload shaders in storage, this function not reload shaders in objects
		void				ReloadShaders();

		// Reload texture in storage, this function not reload textures in objects
		void				ReloadTextures();

		void				DumpAllResources();

	private:
		bool				ConstructPath(std::string filename, std::string type, std::string ex, std::string& resultpath);

		// TODO: Smart pointer
		std::map<std::pair<std::string, std::type_index>, Resource*>	m_storage;
		std::string m_dataFolderName;

	public:
		// Create blank resource 
		template <typename T>
		T* Create(std::string name = std::string(), bool cantDelete = false)
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
		bool Add(T* resource, std::string name = std::string(), bool cantDelete = false)
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
			else if(Core::Utils::isStringEmpty(resource->m_name))
			{
				resource->m_name = name;
			}

			resource->m_cantDelete = cantDelete;
			auto key = std::make_pair(Core::Utils::isStringEmpty(name) ? resource->m_name : name, std::type_index(typeid(T)));
			m_storage[key] = resource;

			return true;
		}

		// Load resource from data folder
		template <typename T>
		bool Load(std::string filename, std::string name = std::string(), bool cantDelete = false)
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

		// Erase resource from storage
		template <typename T>
		void Erase(std::string name)
		{
			auto key = std::make_pair(name, std::type_index(typeid(T)));
			auto it = m_storage.find(key);
			if (it != m_storage.end())
			{
				if (it->second->m_cantDelete == false)
				{
					Logger::Log("[ResourceManager] Erase %s %s", typeid(T).name(), name.c_str());
					m_storage.erase(key);
				}
			}
		}

		// Get resource from storage
		template <typename T>
		[[nodiscard]] T* Get(std::string name)
		{
			auto key = std::make_pair(name, std::type_index(typeid(T)));
			auto it = m_storage.find(key);
			
			if (it == m_storage.end())
			{
				return std::is_same<T, Render::Texture>::value ? Get<T>("DefaultTexture") : nullptr;
			}
			
			return reinterpret_cast<T*>(it->second);
		}

		// Check resource on exist
		template <typename T>
		[[nodiscard]] bool IsExists(std::string name) const
		{
			auto key = std::make_pair(name, std::type_index(typeid(T)));
			auto it = m_storage.find(key);
			
			return it != m_storage.end();
		}

		// Get count of resource in storage
		template<typename T>
		[[nodiscard]] std::int32_t GetResourceCount() const
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
	};


}

