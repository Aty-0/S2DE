#include "ResourceManager.h"
#include <fstream>


namespace S2DE
{
	ResourceManager::ResourceManager()
	{
		m_resource_data_name = S2DE_DEFAULT_RESOURCE_DATA_NAME;
	}

	ResourceManager::~ResourceManager()
	{
		m_default_texture.reset();

	}

	void ResourceManager::ReloadShaders()
	{
		Logger::Log("[ResourceManager] Reload shaders...");

		for (auto& p : m_ResourceStorage)
		{
			if (p.first.second == std::type_index(typeid(Shader)))
				reinterpret_cast<ResourceManager_Resource<Shader>*>(p.second.get())->Get()->Compile();
		}
	}
	
	void ResourceManager::ReloadTextures()
	{
		S2DE_NO_IMPL();
	}

	bool ResourceManager::LoadDefaultTexture() 
	{
		m_default_texture = std::make_unique<Texture>();
		m_default_texture->SetFileName("default_texture");
		return m_default_texture->Load(GetFilePath("default_texture", m_default_texture.get()));
	}

	void ResourceManager::ClearAll()
	{
		Logger::Log("[ResourceManager] ClearAll");

		m_ResourceStorage.clear();
	}

	bool ResourceManager::TryToFind(std::string filename, std::string type, std::string ex, std::string& resultpath)
	{
		//Final path 
		std::string res = m_resource_data_name + type + "/" + filename + ex;
		std::ifstream f;
		f.open(res.c_str(), std::ifstream::ate | std::ios::binary);

		//Check on exist file by final path
		if (f.is_open())
		{
			Logger::Log("[ResourceManager] %s found | %s", type.c_str(), res.c_str());
	
     		resultpath = res;
			return true;
		}

		return false;
	}

	bool ResourceManager::GetFilePath(std::string filename, IO_File* file, std::string& resultpath)
	{
		std::uint32_t size = (std::uint32_t)file->GetExtension().size();

		for (std::uint32_t i = 0; i <= size - 1; i++)
		{
			if (TryToFind(filename, file->GetType(), file->GetExtension()[i], resultpath))
				return true;			
		}

		Logger::Error("[ResourceManager] Can't find %s %s%s/%s", file->GetType().c_str(), m_resource_data_name.c_str(), file->GetType().c_str(), filename.c_str());
		return false;
	}

	std::string  ResourceManager::GetFilePath(std::string filename, IO_File* file)
	{
		std::string resultpath;

		std::uint32_t size = (std::uint32_t)file->GetExtension().size();

		for (std::uint32_t i = 0; i <= size - 1; i++)
		{
			if (TryToFind(filename, file->GetType(), file->GetExtension()[i], resultpath))
				return resultpath;
		}

		Logger::Error("[ResourceManager] Can't find %s %s%s/%s", file->GetType().c_str(), m_resource_data_name.c_str(),  file->GetType().c_str(), filename.c_str());

		return std::string();
	}

	bool ResourceManager::GetFilePath(std::string filename, std::string type, std::string ex[], std::string& resultpath)
	{
		IO_File* file = new IO_File(type, ex);
		return GetFilePath(filename, file, resultpath);
	}

	bool ResourceManager::GetFilePath(std::string filename, std::string type, std::string ex, std::string& resultpath)
	{
		IO_File* file = new IO_File(type, ex);
		return GetFilePath(filename, file, resultpath);
	}

}