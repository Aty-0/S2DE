#include "SceneManager.h"

#include "Render/Renderer.h"

#include "GameObjects/Components/Skybox.h"
#include "GameObjects/Components/Camera.h"
#include "GameObjects/Components/UI/UIDrawableComponent.h"
#include "GameObjects/Components/DrawableComponent.h"
#include "GameObjects/Components/AlphaComponent.h"
#include "GameObjects/Components/Editor/EditorGrid.h"
#include "GameObjects/Components/Editor/EditorCenterCursor.h"

#include <boost/range/adaptor/reversed.hpp>

namespace S2DE::Scene
{
	SceneManager::SceneManager() : 
		m_scene(nullptr),
		m_update_enabled(true),
		m_render_enabled(true),
		m_render_imgui_enabled(true)
	{

	}

	SceneManager::~SceneManager()
	{
		Core::Delete(m_scene);
	}

	void SceneManager::CreateNewScene()
	{	 
		m_scene = new Scene();
		auto camera = CreateGameObject<GameObjects::GameObject>(S2DE_MAIN_CAMERA_NAME, S2DE_ENGINE_GAMEOBJECT_TYPE, -1);
		camera->CreateComponent<GameObjects::Components::Camera>();
		auto sky = CreateGameObject<GameObjects::GameObject>("Sky", S2DE_ENGINE_GAMEOBJECT_TYPE, -1);
		sky->CreateComponent<GameObjects::Components::Skybox>();
		
		if (Core::Engine::isEditor())
		{
			auto editorGrid = CreateGameObject<GameObjects::GameObject>("_EditorGrid", S2DE_ENGINE_GAMEOBJECT_TYPE, -1, DirectX::SimpleMath::Vector3(GRID_CELLS / 2, -1, GRID_CELLS / 2));
			editorGrid->CreateComponent<GameObjects::Components::Editor::EditorGrid>();

			auto editorCenterCursor = CreateGameObject<GameObjects::GameObject>("_EditorCenterCursor", S2DE_ENGINE_GAMEOBJECT_TYPE, -1);
			editorCenterCursor->CreateComponent<GameObjects::Components::Editor::EditorCenterCursor>();
		}
	}	 
		 
	bool SceneManager::LoadScene(std::string name)
	{	 
		S2DE_NO_IMPL();
		return true;
	}	 
		 
	bool SceneManager::SaveScene()
	{	 
		S2DE_NO_IMPL();
		return true;
	}	 

	void SceneManager::UpdateShaders()
	{
		for (const auto& object : m_scene->GetStorage())
		{
			auto components = object.second.get()->GetComponent<GameObjects::Components::DrawableComponent>();

			if (components != nullptr)
			{
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

				GameObjects::Components::AlphaComponent* alphaComponent = gameObject->GetComponent<GameObjects::Components::AlphaComponent>();

				if (alphaComponent != nullptr)
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

				auto uiComponent = gameObject->GetComponent<GameObjects::Components::UI::UIDrawableComponent>();

				if (uiComponent != nullptr)
				{
					gameObject->Render();
				}
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

	void SceneManager::ToggleImGUIVisibility()
	{
		m_render_imgui_enabled =! m_render_imgui_enabled;
	}

	void SceneManager::ToggleGameObjectUpdate()
	{
		m_update_enabled =! m_update_enabled;
	}

	void SceneManager::Clear()
	{
		m_scene->Destroy();
		Logger::Log("[SceneManager] Current scene deleted");
	}
}