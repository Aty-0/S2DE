#include "ResourceManager.h"
#include <fstream>

namespace S2DE::Core::Resources
{
	ResourceManager::ResourceManager()
	{
		m_dataFolderName = S2DE_DEFAULT_RESOURCE_DATA_NAME;
	}

	ResourceManager::~ResourceManager()
	{
		m_storage.clear();
	}

	void ResourceManager::ReloadShaders()
	{
		Logger::Log("[ResourceManager] Reload shaders...");

		for (const auto& p : m_storage)
		{
			if (p.first.second == std::type_index(typeid(Render::Shader)))
			{
				reinterpret_cast<Render::Shader*>(p.second)->Reload();
			}
		}
	}
	
	void ResourceManager::ReloadTextures()
	{
		Logger::Log("[ResourceManager] Reload textures...");

		for (const auto& p : m_storage)
		{
			if (p.first.second == std::type_index(typeid(Render::Texture)))
			{
				const auto texture = reinterpret_cast<Render::Shader*>(p.second);
				texture->Load(texture->GetName());
			}
		}
	}

	void ResourceManager::ClearAll()
	{
		// TODO: Clear resources without m_cantDelete flag
		//Logger::Log("[ResourceManager] Clear all resources");
		//
		//m_storage.clear();
	}

	bool ResourceManager::ConstructPath(std::string filename, std::string type, std::string ex, std::string& resultpath)
	{
		//Construct path
		std::string res = m_dataFolderName + type + "/" + filename + ex;
		//Open file by constructed path
		std::ifstream file = std::ifstream(res.c_str(), std::ifstream::in);		
		//Check on exist file by final path
		if (file.is_open())
		{
			Logger::Log("[ResourceManager] %s found | %s", type.c_str(), res.c_str());
     		resultpath = res;
			file.close();
			return true;
		}

		file.close();
		return false;
	}

	bool ResourceManager::GetFilePath(std::string filename, Resource* file, std::string& resultpath)
	{		
		for(const auto extension : file->GetExtensions())
		{
			if (ConstructPath(filename, file->GetType(), extension, resultpath))
				return true;			
		}

		Logger::Error("[ResourceManager] Can't find %s %s%s/%s", file->GetType().c_str(), m_dataFolderName.c_str(), file->GetType().c_str(), filename.c_str());
		return false;
	}

	std::string  ResourceManager::GetFilePath(std::string filename, Resource* file)
	{
		std::string resultpath = std::string();

		for (const auto extension : file->GetExtensions())
		{
			if (ConstructPath(filename, file->GetType(), extension, resultpath))
				return resultpath;
		}

		Logger::Error("[ResourceManager] Can't find %s %s%s/%s", file->GetType().c_str(), m_dataFolderName.c_str(),  file->GetType().c_str(), filename.c_str());
		return resultpath;
	}

	bool ResourceManager::GetFilePath(std::string filename, std::string type, std::string ex[], std::string& resultpath)
	{
		Resource* file = new Resource(type, ex);
		return GetFilePath(filename, file, resultpath);
	}

	bool ResourceManager::GetFilePath(std::string filename, std::string type, std::string ex, std::string& resultpath)
	{
		Resource* file = new Resource(type, ex);
		return GetFilePath(filename, file, resultpath);
	}

	inline std::string ResourceManager::GetNameOfDataFolder() const
	{
		return m_dataFolderName;
	}


}