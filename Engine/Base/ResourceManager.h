#pragma once
#include "Base/Main/Common.h"
#include "Base/Engine.h"
#include "Base/Resource.h"

namespace S2DE::Render
{
	class Texture;
}

namespace S2DE::Core::Resources
{	
	class S2DE_API ResourceManager : public Core::Utils::Singleton<ResourceManager>
	{
	public:
		static constexpr auto ResourceDataFolderName = "Gamedata/";
		static constexpr auto DefaultResourceName = "__default__";
		
		ResourceManager();
		~ResourceManager();

		// Clear all resources
		void ClearAll();
		// Get file path with Resource class 
		bool GetFilePath(std::string filename, Resource* file, std::string& resultpath);
		// Get file path manualy (Multiply extensions)
		bool GetFilePath(std::string filename, std::string type, std::string ex[], std::string& resultpath);
		// Get file path manualy (Single extension)
		bool GetFilePath(std::string filename, std::string type, std::string ex, std::string& resultpath);
		// Get file path manualy (String)
		std::string GetFilePath(std::string filename, Resource* file);
		// Reload shaders in storage, this function not reload shaders in objects
		void ReloadShaders();
		// Reload texture in storage, this function not reload textures in objects
		void ReloadTextures();
		// Dump resources to log 
		void DumpAllResources();
		// Create blank resource 
		template <typename T>
		T* Create(std::string name = std::string(), bool cantDelete = false);

		template <typename T>
		bool Add(T* resource, std::string name = std::string(), bool cantDelete = false);

		// Load resource from data folder
		template <typename T>
		bool Load(std::string filename, std::string name = std::string(), bool cantDelete = false);

		// Erase resource from storage
		template <typename T>
		void Erase(std::string name);

		// Get resource from storage
		template <typename T>
		[[nodiscard]] T* Get(std::string name = ResourceManager::DefaultResourceName);

		// Check resource on exist
		template <typename T>
		[[nodiscard]] bool IsExists(std::string name) const;

		// Get count of resource in storage
		template <typename T>
		[[nodiscard]] std::int32_t GetResourceCount() const;
	private:
		bool ConstructPath(std::string filename, std::string type, std::string ex, std::string& resultpath);

		// TODO: Smart pointer
		std::map<std::pair<std::string, std::type_index>, Resource*> m_storage;		
	};
}

#include "ResourceManager_inline.h"

