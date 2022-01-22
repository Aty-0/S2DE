#pragma once
#include "Base/Main/Common.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Scene/Scene.h"
#include "GameObjects/GameObject.h"


namespace S2DE::Scene
{
	class S2DE_API SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		//Create new scene 
		void		   CreateNewScene();

		//Load scene from file 
		bool		   LoadScene(std::string name);

		//Save current scene
		bool		   SaveScene();
					   
		void		   RenderImGUI();
		void		   RenderScene();
		void		   UpdateInput();
		void		   UpdateScene(float DeltaTime);
					   
		void		   UpdateShaders();
		void		   UpdateTextures();

		void		   Clear();
					   
		inline Scene*  GetScene() const { return m_scene; }


		void		   ToggleGameObjectVisibility();
		void		   ToggleImGUIVisibility();
		void		   ToggleGameObjectUpdate();
	private:
		Scene*		   m_scene;
					   
		bool		   m_update_enabled;
		bool		   m_render_enabled;
		bool		   m_render_imgui_enabled;
	};

	//Create simple game object in scene
	template<typename T = GameObjects::GameObject>
	static inline T* CreateGameObject(
		std::string name = std::string(),
		std::string type = std::string(),
		std::int32_t prefix = S2DE_DEFAULT_GAMEOBJECT_PREFIX,
		Math::Vector3 position = Math::Vector3::Reset(),
		Math::Vector3 rotation = Math::Vector3::Reset(),
		Math::Vector3 scale = Math::Vector3(1.0f, 1.0f, 1.0f))
	{
		static_assert(!std::is_base_of<T, GameObjects::GameObject>::value || std::is_same<T, GameObjects::GameObject>::value, "This is not GameObject or GameObject based class");

		S2DE_ASSERT(Core::Engine::GetSceneManager()->GetScene() != nullptr);
		auto object = new T();

		object->SetPosition(position);
		object->SetRotation(rotation);
		object->SetScale(scale);

		object->Init(name, type, prefix, std::string());

		Core::Engine::GetSceneManager()->GetScene()->Add<T>(object);

		return object;
	}

	//Create game object without initialize object
	template<typename T = GameObjects::GameObject>
	static inline T* CreateGameObjectNoInit()
	{
		static_assert(!std::is_base_of<T, GameObjects::GameObject>::value || std::is_same<T, GameObjects::GameObject>::value, "This is not GameObject or GameObject based class");

		S2DE_ASSERT(Core::Engine::GetSceneManager()->GetScene() != nullptr);
		auto object = new T();

		Core::Engine::GetSceneManager()->GetScene()->Add<T>(object);

		return object;
	}

	//Get game object from scene by name
	template<typename T = GameObjects::GameObject>
	static inline T* GetObjectByName(std::string name)
	{
		static_assert(!std::is_base_of<T, GameObjects::GameObject>::value || std::is_same<T, GameObjects::GameObject>::value, "This is not GameObject or GameObject based class");

		S2DE_ASSERT(Core::Engine::GetSceneManager()->GetScene() != nullptr);
		SceneObjectStorage::iterator it = std::find_if(
			Core::Engine::GetSceneManager()->GetScene()->GetStorage().begin(), 
			Core::Engine::GetSceneManager()->GetScene()->GetStorage().end(), 
			[&name](std::pair<std::pair<std::string, boost::uuids::uuid>,
				std::shared_ptr<GameObjects::GameObject>> const& elem) {
				return elem.first.first == name;
			});

		return dynamic_cast<T*>(it->second.get());
	}

	//Get game object from scene by uuid
	template<typename T = GameObjects::GameObject>
	static inline T* GetObjectByUUID(boost::uuids::uuid id)
	{
		static_assert(!std::is_base_of<T, GameObjects::GameObject>::value || std::is_same<T, GameObjects::GameObject>::value, "This is not GameObject or GameObject based class");

		S2DE_ASSERT(Core::Engine::GetSceneManager()->GetScene() != nullptr);
		SceneObjectStorage::iterator it = std::find_if(
			Core::Engine::GetSceneManager()->GetScene()->GetStorage().begin(), 
			Core::Engine::GetSceneManager()->GetScene()->GetStorage().end(), 
			[&id](std::pair<std::pair<std::string, boost::uuids::uuid>,
				std::shared_ptr<GameObjects::GameObject>> const& elem) {
					return elem.first.second == id;
			});

		return dynamic_cast<T*>(it->second.get());
	}
}
