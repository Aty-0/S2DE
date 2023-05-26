#include "ResourceManager.h"
#include <fstream>
#include "Render/Shader.h"
#include "Render/Texture.h"
namespace S2DE::Core::Resources
{
	ResourceManager::ResourceManager()
	{

	}

	ResourceManager::~ResourceManager()
	{
		m_storage.clear();
	}

	void ResourceManager::ReloadShaders()
	{
		Utils::Logger::LogColored(DirectX::SimpleMath::Color(0.2f, 1.0f, 0.5f, 1.0f), "[ResourceManager] Reload shaders...");

		for (const auto& p : m_storage)
		{
			if (p.first.second == std::type_index(typeid(Render::Shader)))
			{
				reinterpret_cast<Render::Shader*>(p.second)->Reload();
			}
		}
	}

	void ResourceManager::DumpAllResources()
	{
		Utils::Logger::LogColored(DirectX::SimpleMath::Color(0.2f, 1.0f, 0.5f, 1.0f), " ! ! ! DumpAllResources ! ! !");

		if (m_storage.size() == 0)
		{
			Utils::Logger::Log("Storage is empty!");
			return;
		}


		for (const auto& p : m_storage)
		{
			Utils::Logger::LogColored(DirectX::SimpleMath::Color(0.2f, 1.0f, 0.5f, 1.0f), "RMn: %s Rn: %s T:", p.first.first.c_str(), p.second->m_name.c_str(), p.second->GetType().c_str());

			std::string types_str = std::string();
			for (const auto n : p.second->GetExtensions())
			{
				types_str += " " + n;
			}

			Utils::Logger::LogColored(DirectX::SimpleMath::Color(0.2f, 1.0f, 0.5f, 1.0f), "%s", types_str.c_str());
		}
	}

	void ResourceManager::ReloadTextures()
	{
		Utils::Logger::LogColored(DirectX::SimpleMath::Color(0.2f, 1.0f, 0.5f, 1.0f), "[ResourceManager] Reload textures...");

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
		Utils::Logger::Log("[ResourceManager] Clear all resources");
		for (std::map<std::pair<std::string, std::type_index>, Resource*>::iterator it = m_storage.begin(); it != m_storage.end();)
		{
			if (it->second->m_cantDelete == false)
			{
				it = m_storage.erase(it);
			}
			else
			{
				it++;
			}
		}

		// check clear func 
#ifdef _DEBUG
		DumpAllResources();
#endif
	}

	bool ResourceManager::ConstructPath(std::string filename, std::string type, std::string ex, std::string& resultpath)
	{
		// Construct path
		const auto res = ResourceManager::ResourceDataFolderName + type + "/" + filename + ex;
		// Open file by constructed path
		auto file = std::ifstream(res.c_str(), std::ifstream::in);
		// If file opened we are save the result path 
		if (file.is_open())
		{
			Utils::Logger::LogColored(DirectX::SimpleMath::Color(0.2f, 1.0f, 0.5f, 1.0f), "[ResourceManager] %s found | %s", type.c_str(), res.c_str());
			resultpath = res;

			file.close();
			return true;
		}

		file.close();
		return false;
	}

	bool ResourceManager::GetFilePath(std::string filename, Resource* file, std::string& resultpath)
	{
		for (const auto extension : file->GetExtensions())
		{
			if (ConstructPath(filename, file->GetType(), extension, resultpath))
				return true;
		}

		Utils::Logger::Error("[ResourceManager] Can't find %s %s%s/%s", file->GetType().c_str(), ResourceManager::ResourceDataFolderName, file->GetType().c_str(), filename.c_str());
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

		Utils::Logger::Error("[ResourceManager] Can't find %s %s%s/%s", file->GetType().c_str(), ResourceManager::ResourceDataFolderName, file->GetType().c_str(), filename.c_str());
		return resultpath;
	}

	bool ResourceManager::GetFilePath(std::string filename, std::string type, std::string ex[], std::string& resultpath)
	{
		// FIX ME: memory leak 
		Resource* file = new Resource(type, ex);
		return GetFilePath(filename, file, resultpath);
	}

	bool ResourceManager::GetFilePath(std::string filename, std::string type, std::string ex, std::string& resultpath)
	{
		// FIX ME: memory leak 
		Resource* file = new Resource(type, ex);
		return GetFilePath(filename, file, resultpath);
	}
}