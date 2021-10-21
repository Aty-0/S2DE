#pragma once
#include "Base/Main/Common.h"

///////////////////////////////////
///Resources headers
///////////////////////////////////
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"


#define S2DE_DEFAULT_RESOURCE_DATA_NAME "Gamedata/"

namespace S2DE
{
	class S2DE_API ResourceManager_ResourceBase
	{
	public:
		virtual ~ResourceManager_ResourceBase() { }
		virtual bool Load(std::string name) const = 0;
		virtual bool Save(std::string name, void* output) const = 0;
		virtual void Clear() = 0;
		virtual void CreateResource() = 0;
	};

	template<class T>
	class S2DE_API ResourceManager_Resource : public ResourceManager_ResourceBase
	{
	public:
		ResourceManager_Resource() :
			m_resource(nullptr)
		{

		}

		virtual ~ResourceManager_Resource() override
		{
	
		}

		virtual void Clear() override
		{
			static_assert(!std::is_base_of<T, IO_Disposible>::value, "It's not IO_Disposible based class");

			m_resource->Cleanup();
			Delete(m_resource);
		}

		virtual bool Load(std::string name) const override
		{
			static_assert(!std::is_base_of<T, IO_File>::value, "It's not IO_File based class");

			m_resource = new T();
			m_resource->SetFileName(name);

			//If we have diffrent load functions 
			if (std::is_same<T, Shader>::value)
			{
				if (!reinterpret_cast<Shader*>(m_resource)->SetPaths(
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

		virtual bool Save(std::string name, void* output) const override
		{
			static_assert(!std::is_base_of<T, IO_File>::value, "It's not IO_File based class");

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
	public:
		ResourceManager();
		~ResourceManager();

		//TODO 
		//Put default texture in engine resources
		//Load it from resource.h
		
		//Load default texture
		bool				LoadDefaultTexture();
		//Clear all resources
		void				ClearAll();

		//Get default texture
		inline Texture*		GetDefaultTexture() const { return m_default_texture.get(); }
		//Get data folder name
		inline std::string  GetNameOfData() const { return m_resource_data_name; }

		//Get file path (std::string)
		std::string			GetFilePath(std::string filename, IO_File* file);		
		//Get file path (bool)
		bool				GetFilePath(std::string filename, IO_File* file, std::string& resultpath);
		//Get file path without IO_File class
		//Array extension 
		bool				GetFilePath(std::string filename, std::string type, std::string ex[], std::string& resultpath);
		//Get file path without IO_File class 
		bool				GetFilePath(std::string filename, std::string type, std::string ex, std::string& resultpath);

		//Reload shaders in storage, this function not reload shader in objects
		void				ReloadShaders();

		//Reload texture in storage, this function not reload texture in objects
		void				ReloadTextures();
	private:

	    std::map<std::pair<std::string, std::type_index>,
			std::unique_ptr<ResourceManager_ResourceBase>>	m_ResourceStorage;

		std::unique_ptr <Texture>			m_default_texture;
		std::string						    m_resource_data_name;
	private:
		//Find funtion
		//Create path and check file by created path
		bool TryToFind(std::string filename, std::string type, std::string ex, std::string& resultpath);

	public:
		//Load resource in Data
		//T is require class with IO_File base
		template <typename T>
		bool Load(std::string filename, std::string name = std::string())
		{
			if (isStringEmpty(filename))
				return false;

			ResourceManager_Resource<T>* r = new ResourceManager_Resource<T>();

			if (!r->Load(filename))
			{
				Delete(r);
				return false;
			}

			auto key = std::make_pair(isStringEmpty(name) ? filename : name, std::type_index(typeid(T)));
			m_ResourceStorage[key] = std::make_unique<ResourceManager_Resource<T>>(*r);

			return true;
		}

		//Erase resource in storage
		//T is require class with IO_File base
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
		//T is require class with IO_File base
		template <typename T>
		T* Get(std::string name)
		{
			auto key = std::make_pair(name, std::type_index(typeid(T)));
			auto it = m_ResourceStorage.find(key);

			if (it == m_ResourceStorage.end())
				return std::is_same<T, Texture>::value ? reinterpret_cast<T*>(Engine::GetResourceManager().GetDefaultTexture()) : nullptr;
	
			return reinterpret_cast<ResourceManager_Resource<T>*>(it->second.get())->Get();
		}

		//Check resource on exist
		//T is require class with IO_File base
		template <typename T>
		bool IsExists(std::string name) const
		{
			auto key = std::make_pair(name, std::type_index(typeid(T)));
			auto it = m_ResourceStorage.find(key);

			return it != m_ResourceStorage.end();
		}

		//FIX ME
		//I think it's wrong implementation

		//Get count of resource in storage
		//T is require class with IO_File base
		template<typename T>
		std::int32_t GetResourceCount() const
		{
			std::int32_t count = 0;

			for (auto& p : m_ResourceStorage)
			{ 
				if (p.first.second == std::type_index(typeid(T)))
					count++;
			}
			
			return count;
		}
	};


}

