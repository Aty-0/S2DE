#pragma once
#include "Base/Main/Common.h"
#include "Base/Engine.h"

///////////////////////////////////
///Resources headers
///////////////////////////////////
#include "IO/IO_File.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/Mesh.h"


#define S2DE_DEFAULT_RESOURCE_DATA_NAME "Gamedata/"

using namespace S2DE::IO;

namespace S2DE::Core
{
	class S2DE_API RMResourceBase
	{
	public:
		virtual ~RMResourceBase() { }
		virtual bool Load(std::string name) const = 0;
		virtual void Clear() = 0;
		virtual void CreateResource() = 0;
	};

	template<class T>
	class S2DE_API RMResource : public RMResourceBase
	{
	public:
		RMResource() :
			m_resource(nullptr)
		{
			static_assert(!std::is_base_of<T, IO_File>::value, "It's not IO_File based class");
			static_assert(!std::is_base_of<T, IO_Disposible>::value, "It's not IO_Disposible based class");
		}

		virtual ~RMResource() override
		{
	
		}

		virtual void Clear() override
		{
			m_resource->Cleanup();
			Delete(m_resource);
		}

		virtual bool Load(std::string name) const override
		{		
			m_resource = new T();
			m_resource->SetFileName(name);

			//If we have diffrent load functions 
			if (std::is_same<T, Render::Shader>::value)
			{
				if (!reinterpret_cast<Render::Shader*>(m_resource)->SetPaths(
					Engine::GetResourceManager().GetFilePath(name + "_vs", reinterpret_cast<T*>(m_resource)),
					Engine::GetResourceManager().GetFilePath(name + "_ps", reinterpret_cast<T*>(m_resource))
				))
				{
					return false;
				}
			}
			else
			{
				if (!m_resource->Load(Engine::GetResourceManager().GetFilePath(name, reinterpret_cast<T*>(m_resource))))
				{
					return false;
				}
			}


			return true;
		}

		virtual void CreateResource() override
		{
			m_resource = new T();
		}

		inline T* Get() const { return m_resource; }

	private:
		mutable T* m_resource;
	};

	class S2DE_API ResourceManager
	{
	private:
		std::map<std::pair<std::string, std::type_index>, std::unique_ptr<RMResourceBase>>	m_ResourceStorage;
		std::unique_ptr <Render::Texture>			m_defaultTexture;
		std::string									m_dataFolderName;
	public:
		ResourceManager();
		~ResourceManager();

		//Load default texture
		bool				LoadDefaultTexture();
		//Clear all resources
		void				ClearAll();
		//Get default texture
		inline Render::Texture*		GetDefaultTexture() const { return m_defaultTexture.get(); }
		//Get data folder name
		inline std::string  GetNameOfDataFolder() const { return m_dataFolderName; }

		std::string			GetFilePath(std::string filename, IO_File* file);		
		bool				GetFilePath(std::string filename, IO_File* file, std::string& resultpath);
		bool				GetFilePath(std::string filename, std::string type, std::string ex[], std::string& resultpath);
		bool				GetFilePath(std::string filename, std::string type, std::string ex, std::string& resultpath);

		//Reload shaders in storage, this function not reload shaders in objects
		void				ReloadShaders();

		//Reload texture in storage, this function not reload textures in objects
		void				ReloadTextures();

	private:
		bool				ConstructPath(std::string filename, std::string type, std::string ex, std::string& resultpath);

	public:
		//Load resource from data folder
		template <typename T>
		bool Load(std::string filename, std::string name = std::string())
		{
			if (Core::Utils::isStringEmpty(filename))
				return false;

			RMResource<T>* r = new RMResource<T>();

			if (!r->Load(filename))
			{
				Delete(r);
				return false;
			}

			auto key = std::make_pair(Core::Utils::isStringEmpty(name) ? filename : name, std::type_index(typeid(T)));
			m_ResourceStorage[key] = std::make_unique<RMResource<T>>(*r);

			return true;
		}

		//Erase resource from storage
		template <typename T>
		void Erase(std::string name)
		{
			auto key = std::make_pair(name, std::type_index(typeid(T)));
			auto it = m_ResourceStorage.find(key);
			if (it != m_ResourceStorage.end())
			{
				Logger::Log("[ResourceManager] Erase %s %s", typeid(T).name(), name.c_str());

				it->second->Clear();
				m_ResourceStorage.erase(key);
			}
		}

		//Get resource from storage
		template <typename T>
		T* Get(std::string name)
		{
			auto key = std::make_pair(name, std::type_index(typeid(T)));
			auto it = m_ResourceStorage.find(key);

			if (it == m_ResourceStorage.end())
				return std::is_same<T, Render::Texture>::value ? reinterpret_cast<T*>(Engine::GetResourceManager().GetDefaultTexture()) : nullptr;
	
			return reinterpret_cast<RMResource<T>*>(it->second.get())->Get();
		}

		//Check resource on exist
		template <typename T>
		bool IsExists(std::string name) const
		{
			auto key = std::make_pair(name, std::type_index(typeid(T)));
			auto it = m_ResourceStorage.find(key);

			return it != m_ResourceStorage.end();
		}

		//Get count of resource in storage
		template<typename T>
		std::int32_t GetResourceCount() const
		{
			std::int32_t count = 0;

			for (auto& p : m_ResourceStorage) 
				if (p.first.second == std::type_index(typeid(T)))
					count++;
			
			return count;
		}
	};


}

