#pragma once
#include "Base/Main/Common.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Scene/Scene.h"

namespace S2DE::Render
{
	class Renderer;
}

namespace S2DE::GameObjects
{
	class GameObject;
}

namespace S2DE::Scene
{
	class S2DE_API SceneManager : public Core::Utils::Singleton<SceneManager>
	{
	public:
		SceneManager();
		~SceneManager();

		// Create new scene 
		void CreateNewScene();
		// Load scene from file 
		bool LoadScene(std::string name);
		// Save current scene
		bool SaveScene();
		// Render game objects 
		void RenderScene(Render::Renderer* renderer);
		// Update game objects 
		void UpdateScene(float DeltaTime);
		// Calls for all drawable objects update shader
		void UpdateShaders();
		// Calls for all drawable objects update texture
		void UpdateTextures();
		// Destroy current scene 
		void Clear();
		// Turn off Update for game objects
		void ToggleGameObjectUpdating();

		[[nodiscard]] inline Scene* GetScene() const;

		// Create game object in scene
		template<typename T = GameObjects::GameObject>
		T* CreateGameObject(
			std::string name = std::string(),
			std::string type = std::string(),
			std::int32_t prefix = S2DE_DEFAULT_GAMEOBJECT_PREFIX,
			Math::float3 position = Math::float3::Zero,
			Math::float3 rotation = Math::float3::Zero,
			Math::float3 scale = Math::float3(1.0f, 1.0f, 1.0f));

		// Get game object from scene by name
		template<typename T = GameObjects::GameObject>
		[[nodiscard]] T* GetObjectByName(std::string name);

		// Get game object from scene by uuid
		template<typename T = GameObjects::GameObject>
		[[nodiscard]] T* GetObjectByUUID(boost::uuids::uuid uuid);

		// Create game object function without initialization 
		template<typename T = GameObjects::GameObject>
		[[nodiscard]] T* CreateGameObjectNoInit();
	private:
		Scene*		   m_scene;
		bool		   m_updateEnabled;
	};
}

#include "SceneManager_inline.h"