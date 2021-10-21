#pragma once
#include "Base/Main/Common.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Scene/Scene.h"
#include "GameObjects/GameObject.h"


namespace S2DE
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
					   
		void		   RenderDebugGUI();
		void		   RenderScene();
		void		   UpdateInput();
		void		   UpdateScene(float DeltaTime);
					   
		void		   UpdateShaders();
		void		   UpdateTextures();

		void		   Clear();
					   
		inline Scene*  GetScene() const { return m_scene; }


		void		   ToggleGameObjectVisibility();
		void		   ToggleDebugGUIVisibility();
		void		   ToggleGameObjectUpdate();
	private:
		Scene*		   m_scene;
					   
		bool		   m_update_enabled;
		bool		   m_render_enabled;
		bool		   m_render_dbg_gui_enabled;
	};

	//Create simple game object in scene
	template<typename T = GameObject>
	static inline T* CreateGameObject(
		std::string name = std::string(),
		std::string type = std::string(),
		std::int32_t prefix = S2DE_DEFAULT_GAMEOBJECT_PREFIX,
		Vector3 position = Vector3::Reset(),
		Vector3 rotation = Vector3::Reset(),
		Vector3 scale = Vector3(1.0f, 1.0f, 1.0f))
	{
		S2DE_ASSERT(Engine::GetSceneManager()->GetScene() != nullptr);
		auto object = new T();

		object->SetPosition(position);
		object->SetRotation(rotation);
		object->SetScale(scale);

		object->Init(name, type, prefix, std::string());

		Engine::GetSceneManager()->GetScene()->Add<T>(object);

		return object;
	}

	//Create game object without initialize object
	template<typename T = GameObject>
	static inline T* CreateGameObjectNoInit()
	{
		S2DE_ASSERT(Engine::GetSceneManager()->GetScene() != nullptr);
		auto object = new T();

		Engine::GetSceneManager()->GetScene()->Add<T>(object);

		return object;
	}

	//Get game object from scene by name
	template<typename T = GameObject>
	static inline T* GetObjectByName(std::string name)
	{
		S2DE_ASSERT(Engine::GetSceneManager()->GetScene() != nullptr);
		SceneObjectStorage::iterator it = std::find_if(
			Engine::GetSceneManager()->GetScene()->GetStorage().begin(), 
			Engine::GetSceneManager()->GetScene()->GetStorage().end(), 
			[&name](std::pair<std::pair<std::string, boost::uuids::uuid>,
				std::shared_ptr<GameObject>> const& elem) {
				return elem.first.first == name;
			});

		return dynamic_cast<T*>(it->second.get());
	}

	//Get game object from scene by uuid
	template<typename T = GameObject>
	static inline T* GetObjectByUUID(boost::uuids::uuid id)
	{
		S2DE_ASSERT(Engine::GetSceneManager()->GetScene() != nullptr);
		SceneObjectStorage::iterator it = std::find_if(
			Engine::GetSceneManager()->GetScene()->GetStorage().begin(), 
			Engine::GetSceneManager()->GetScene()->GetStorage().end(), 
			[&id](std::pair<std::pair<std::string, boost::uuids::uuid>,
				std::shared_ptr<GameObject>> const& elem) {
					return elem.first.second == id;
			});

		return dynamic_cast<T*>(it->second.get());
	}
}