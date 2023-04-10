#include "SceneManager.h"

#include "Render/Renderer.h"

#include "GameObjects/Base/GameObject.h"
#include "GameObjects/Components/Skybox.h"
#include "GameObjects/Components/Camera.h"
#include "GameObjects/Components/UI/UIDrawableComponent.h"
#include "GameObjects/Components/DrawableComponent.h"
#include "GameObjects/Components/AlphaComponent.h"
#include "GameObjects/Components/Editor/EditorGrid.h"
#include "GameObjects/Components/Editor/EditorCenterCursor.h"

#include <boost/range/adaptor/reversed.hpp>
#include <sstream>
#include <fstream>

#include "Base/Utils/SerializeUtils.h"
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

namespace S2DE::Scene
{
	SceneManager::SceneManager() : 
		m_scene(nullptr),
		m_update_enabled(true),
		m_render_enabled(true)
	{

	}

	SceneManager::~SceneManager()
	{
		Core::Delete(m_scene);
	}

	void SceneManager::CreateNewScene()
	{	 
		m_scene = new Scene();
		const auto camera = CreateGameObject<GameObjects::GameObject>(S2DE_MAIN_CAMERA_NAME, S2DE_ENGINE_GAMEOBJECT_TYPE, -1);
		camera->CreateComponent<GameObjects::Components::Camera>();
		const auto sky = CreateGameObject<GameObjects::GameObject>("Sky", S2DE_ENGINE_GAMEOBJECT_TYPE, -1);
		sky->CreateComponent<GameObjects::Components::Skybox>();
		
		if (Core::Engine::isEditor())
		{
			const auto editorGrid = CreateGameObject<GameObjects::GameObject>("_EditorGrid", S2DE_ENGINE_GAMEOBJECT_TYPE, -1, DirectX::SimpleMath::Vector3(GRID_CELLS / 2, -1, GRID_CELLS / 2));
			editorGrid->CreateComponent<GameObjects::Components::Editor::EditorGrid>();

			const auto editorCenterCursor = CreateGameObject<GameObjects::GameObject>("_EditorCenterCursor", S2DE_ENGINE_GAMEOBJECT_TYPE, -1);
			editorCenterCursor->CreateComponent<GameObjects::Components::Editor::EditorCenterCursor>();
		}

		// TODO: Callbacks
	}	 
		 
	bool SceneManager::LoadScene(std::string name)
	{	 
		S2DE_NO_IMPL();
		return true;
	}	 

	bool SceneManager::SaveScene()
	{	 
		Logger::Log("[SceneManager] Save scene");
		boost::archive::xml_oarchive oa(m_os);

		for (const auto& object : m_scene->GetStorage())
		{
			oa << boost::serialization::make_nvp(object.first.first.c_str(), static_cast<GameObjects::GameObject&>(*object.second.get()));
		}	

		//PrintSerializedObjects();
		
		std::ofstream file;

		file.open("scene.xml");
		file << m_os.str();
		file.close();

		return true;
	}	 

	void SceneManager::UpdateShaders()
	{
		for (const auto& object : m_scene->GetStorage())
		{
			auto components = object.second.get()->GetComponent<GameObjects::Components::DrawableComponent>();

			if (components != nullptr)
			{
				Logger::Log("Update shader for %s", object.first.first.c_str());
				components->UpdateShader();
			}
		}
	}

	void SceneManager::UpdateTextures()
	{
		for (const auto& object : m_scene->GetStorage())
		{
			auto components = object.second.get()->GetComponent<GameObjects::Components::DrawableComponent>();

			if (components != nullptr)
			{
				components->UpdateTexture();
			}
		}
	}
		 	 
	void SceneManager::RenderScene()
	{
		if (m_render_enabled && m_scene)
		{
			//for (const auto& object : m_scene->GetStorage())
			//{
			//	auto gameObject = object.second.get();
			//	if (gameObject == nullptr)
			//		continue;
			//	gameObject->Render();
			//}

			for (const auto& object : m_scene->GetStorage())
			{
				auto gameObject = object.second.get();
				if (gameObject == nullptr)
					continue;

				GameObjects::Components::AlphaComponent* alphaComponent = gameObject->GetComponent<GameObjects::Components::AlphaComponent>();

				if (alphaComponent == nullptr)
				{
					gameObject->Render();
				}
			}

			Core::Engine::GetRenderer()->TurnOnAlphaBlending();
			for (const auto& object : boost::adaptors::reverse(m_scene->GetStorage()))
			{
				auto gameObject = object.second.get();
				if (gameObject == nullptr)
					continue;

				auto drawComponent = gameObject->GetComponent<GameObjects::Components::DrawableComponent>();
				auto alphaComponent = gameObject->GetComponent<GameObjects::Components::AlphaComponent>();

				if (drawComponent != nullptr && 
					alphaComponent != nullptr)
				{
					gameObject->Render();
				}
			}
			Core::Engine::GetRenderer()->TurnOffAlphaBlending();

			Core::Engine::GetRenderer()->TurnZBufferOff();
			for (const auto& object : m_scene->GetStorage())
			{
				auto gameObject = object.second.get();
				if (gameObject == nullptr)
					continue;

				auto alphaComponent = gameObject->GetComponent<GameObjects::Components::AlphaComponent>();

				if (alphaComponent != nullptr)
					Core::Engine::GetRenderer()->TurnOnAlphaBlending();

				auto uiComponent = gameObject->GetComponent<GameObjects::Components::UI::UIDrawableComponent>();

				if (uiComponent != nullptr)
				{
					gameObject->Render();
				}

				if (alphaComponent != nullptr)
					Core::Engine::GetRenderer()->TurnOffAlphaBlending();
			}
			Core::Engine::GetRenderer()->TurnZBufferOn();
		}
	}

	void SceneManager::UpdateScene(float DeltaTime)
	{
		if (m_update_enabled && m_scene)
		{
			for (const auto& object : m_scene->GetStorage())
			{
				object.second.get()->Update(DeltaTime);
			}
		}
	}

	void SceneManager::ToggleGameObjectVisibility()
	{
		m_render_enabled =! m_render_enabled;
	}

	void SceneManager::ToggleGameObjectUpdating()
	{
		m_update_enabled =! m_update_enabled;
	}

	inline Scene* SceneManager::GetScene() const
	{
		return m_scene;
	}

	void SceneManager::Clear()
	{
		m_scene->Destroy();
		Logger::Log("[SceneManager] Current scene deleted");
	}
}