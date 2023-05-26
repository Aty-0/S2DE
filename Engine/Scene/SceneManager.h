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

	private:
		Scene*		   m_scene;
		bool		   m_updateEnabled;
	};

	// FIX ME: Need to relocate this functions, where ?

	// Get game object from scene by name
	template<typename T = GameObjects::GameObject>
	[[nodiscard]] static T* GetObjectByName(std::string name)
	{
		static_assert(!std::is_base_of<T, GameObjects::GameObject>::value || std::is_same<T, GameObjects::GameObject>::value, "This is not GameObject or GameObject based class");
		static const auto sceneManager = SceneManager::GetInstance();

		Assert(sceneManager->GetScene() != nullptr, "Scene is nullptr");

		auto storage = sceneManager->GetScene()->GetStorage();
		SceneObjectStorage::iterator it = std::find_if(
			storage.begin(), storage.end(), 
			[&name](std::pair<std::pair<std::string, boost::uuids::uuid>,
				std::shared_ptr<GameObjects::GameObject>> const& elem) {
				return elem.first.first == name;
			});

		return dynamic_cast<T*>(it->second.get());
	}

	// Get game object from scene by uuid
	template<typename T = GameObjects::GameObject>
	[[nodiscard]] static T* GetObjectByUUID(boost::uuids::uuid uuid)
	{
		static_assert(!std::is_base_of<T, GameObjects::GameObject>::value || std::is_same<T, GameObjects::GameObject>::value, "This is not GameObject or GameObject based class");
		static const auto sceneManager = SceneManager::GetInstance();

		Assert(sceneManager->GetScene() != nullptr, "Scene is nullptr");

		auto storage = sceneManager->GetScene()->GetStorage();
		SceneObjectStorage::iterator it = std::find_if(
			storage.begin(), storage.end(), 
			[&uuid](std::pair<std::pair<std::string, boost::uuids::uuid>,
				std::shared_ptr<GameObjects::GameObject>> const& elem) {
					return elem.first.second == uuid;
			});

		return dynamic_cast<T*>(it->second.get());
	}

	// Create game object in scene
	template<typename T = GameObjects::GameObject>
	static T* CreateGameObject(
		std::string name = std::string(),
		std::string type = std::string(),
		std::int32_t prefix = S2DE_DEFAULT_GAMEOBJECT_PREFIX,
		Math::float3 position = Math::float3::Zero,
		Math::float3 rotation = Math::float3::Zero,
		Math::float3 scale    = Math::float3(1.0f, 1.0f, 1.0f))
	{
		static_assert(!std::is_base_of<T, GameObjects::GameObject>::value || std::is_same<T, GameObjects::GameObject>::value, 
			"This is not GameObject or GameObject based class");

		static const auto sceneManager = SceneManager::GetInstance();

		// Without scene we can't continue 
		Assert(sceneManager->GetScene() != nullptr, "Scene is nullptr");

		const auto object = new T(name, type, prefix, std::string());
		const auto transform = object->GetTransform();

		// If we get there assertion fail it means something very bad happened. 
		Assert(transform, "Transform component is nullptr, wat ?");

		transform->SetPosition(position);
		transform->SetRotation(rotation);
		transform->SetScale(scale);
		
		return sceneManager->GetScene()->Add<T>(object);
	}

	// Create game object function without initialization 
	template<typename T = GameObjects::GameObject>
	static T* CreateGameObjectNoInit()
	{
		static_assert(!std::is_base_of<T, GameObjects::GameObject>::value || std::is_same<T, GameObjects::GameObject>::value, 
			"This is not GameObject or GameObject based class");
		static const auto sceneManager = SceneManager::GetInstance();

		// Without scene we can't continue 
		Assert(sceneManager->GetScene() != nullptr, "Scene is nullptr");

		const auto object = new T();
		return sceneManager->GetScene()->Add<T>(object);
	}
}
