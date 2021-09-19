#pragma once
#include "Base/Main/Common.h"
#include "Scene/Scene.h"
#include "GameObjects/GameObject.h"


namespace S2DE
{
	class S2DE_API SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		void		  CreateNewScene();
		bool		  LoadScene(std::string name);
		bool		  SaveScene();


		void		  RenderDebugGUI();
		void		  RenderScene();
		void		  UpdateInput();
		void		  UpdateScene(float DeltaTime);

		void		  Clear();

		inline Scene* GetScene() const { return m_scene; }

	private:
		Scene* m_scene;
	};

	//Create simple game object in scene
	template<typename T = GameObject>
	static inline T* CreateGameObject(
		std::string name = "NewObject",
		std::string type = "GameObject",
		std::int32_t prefix = 1,
		Vector3 position = Vector3::Zero(),
		Vector3 rotation = Vector3::Zero(),
		Vector3 scale = Vector3(1.0f, 1.0f, 1.0f))
	{
		S2DE_ASSERT(Engine::GetSceneManager()->GetScene() != nullptr);
		auto object = new T();

		object->Init(name, type, prefix, std::string());
		object->SetPosition(position);
		object->SetRotation(rotation);
		object->SetScale(scale);

		Engine::GetSceneManager()->GetScene()->Add(object);

		return object;
	}

	//Create game object without initialize object
	template<typename T = GameObject>
	static inline T* CreateGameObjectNoInit()
	{
		S2DE_ASSERT(Engine::GetSceneManager()->GetScene() != nullptr);
		auto object = new T();

		Engine::GetSceneManager()->GetScene()->Add(object);

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
				GameObject*> const& elem) {
				return elem.first.first == name;
			});

		return dynamic_cast<T*>(it->second);
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
				GameObject*> const& elem) {
					return elem.first.second == id;
			});

		return dynamic_cast<T*>(it->second);
	}
}
