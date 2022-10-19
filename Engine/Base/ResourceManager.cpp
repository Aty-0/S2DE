#include "ResourceManager.h"
#include <fstream>

namespace S2DE::Core
{
	ResourceManager::ResourceManager()
	{
		m_dataFolderName = S2DE_DEFAULT_RESOURCE_DATA_NAME;
	}

	ResourceManager::~ResourceManager()
	{
		m_defaultTexture.reset();
	}

	void ResourceManager::ReloadShaders()
	{
		Logger::Log("[ResourceManager] Reload shaders...");

		for (const auto& p : m_ResourceStorage)
		{
			if (p.first.second == std::type_index(typeid(Render::FR::Shader)))
				reinterpret_cast<RMResource<Render::FR::Shader>*>(p.second.get())->Get()->Reload();
		}
	}
	
	void ResourceManager::ReloadTextures()
	{
		Logger::Log("[ResourceManager] Reload textures...");

		for (const auto& p : m_ResourceStorage)
		{
			if (p.first.second == std::type_index(typeid(Render::FR::Texture)))
			{
				auto t = reinterpret_cast<RMResource<Render::FR::Texture>*>(p.second.get())->Get();
				t->Load(GetFilePath(t->GetName(), t));
			}
		}
	}

	bool ResourceManager::LoadDefaultTexture() 
	{
		m_defaultTexture = std::make_unique<Render::FR::Texture>();
		m_defaultTexture->SetFileName("default_texture");
		return m_defaultTexture->Load(GetFilePath("default_texture", m_defaultTexture.get()));
	}

	void ResourceManager::ClearAll()
	{
		Logger::Log("[ResourceManager] Clear all resources");

		m_ResourceStorage.clear();
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

	bool ResourceManager::GetFilePath(std::string filename, IO_File* file, std::string& resultpath)
	{
		std::uint32_t size = (std::uint32_t)file->GetExtension().size();

		for (std::uint32_t i = 0; i <= size - 1; i++)
		{
			if (ConstructPath(filename, file->GetType(), file->GetExtension()[i], resultpath))
				return true;			
		}

		Logger::Error("[ResourceManager] Can't find %s %s%s/%s", file->GetType().c_str(), m_dataFolderName.c_str(), file->GetType().c_str(), filename.c_str());
		return false;
	}

	std::string  ResourceManager::GetFilePath(std::string filename, IO_File* file)
	{
		std::string resultpath = std::string();
		std::uint32_t size = (std::uint32_t)file->GetExtension().size();

		for (std::uint32_t i = 0; i <= size - 1; i++)
		{
			if (ConstructPath(filename, file->GetType(), file->GetExtension()[i], resultpath))
				return resultpath;
		}

		Logger::Error("[ResourceManager] Can't find %s %s%s/%s", file->GetType().c_str(), m_dataFolderName.c_str(),  file->GetType().c_str(), filename.c_str());
		return resultpath;
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