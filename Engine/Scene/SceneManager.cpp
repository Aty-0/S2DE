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
		m_updateEnabled(true)
	{

	}

	SceneManager::~SceneManager()
	{
		Core::Delete(m_scene);
	}

	void SceneManager::CreateNewScene()
	{	 
		m_scene = new Scene();
		CreateGameObject<GameObjects::GameObject>(GameObjects::Components::Camera::EngineCameraName, S2DE_ENGINE_GAMEOBJECT_TYPE, -1)->CreateComponent<GameObjects::Components::Camera>();;
		CreateGameObject<GameObjects::GameObject>("Sky", S2DE_ENGINE_GAMEOBJECT_TYPE, -1)->CreateComponent<GameObjects::Components::Skybox>();

		if (Core::Engine::isEditor())
		{
			CreateGameObject<GameObjects::GameObject>("__Editor_Grid__", S2DE_ENGINE_GAMEOBJECT_TYPE, -1, Math::float3(GRID_CELLS / 2, -1, GRID_CELLS / 2))->CreateComponent<GameObjects::Components::Editor::EditorGrid>();;
			CreateGameObject<GameObjects::GameObject>("__Editor_Center_Cursor__", S2DE_ENGINE_GAMEOBJECT_TYPE, -1)->CreateComponent<GameObjects::Components::Editor::EditorCenterCursor>();
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
		Core::Utils::Logger::Log("[SceneManager] Save scene");
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
				Core::Utils::Logger::Log("Update shader for %s", object.first.first.c_str());
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
		 	 
	void SceneManager::RenderScene(Render::Renderer* renderer)
	{
		if (m_scene)
		{
			for (const auto& object : m_scene->GetStorage())
			{
				auto gameObject = object.second.get();
				if (gameObject == nullptr)
					continue;

				GameObjects::Components::AlphaComponent* alphaComponent = gameObject->GetComponent<GameObjects::Components::AlphaComponent>();

				if (alphaComponent == nullptr)
				{
					gameObject->Render(renderer);
				}
			}

			renderer->TurnOnAlphaBlending();
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
					gameObject->Render(renderer);
				}
			}
			renderer->TurnOffAlphaBlending();

			renderer->TurnZBufferOff();
			for (const auto& object : m_scene->GetStorage())
			{
				auto gameObject = object.second.get();
				if (gameObject == nullptr)
					continue;

				auto alphaComponent = gameObject->GetComponent<GameObjects::Components::AlphaComponent>();

				if (alphaComponent != nullptr)
					renderer->TurnOnAlphaBlending();

				auto uiComponent = gameObject->GetComponent<GameObjects::Components::UI::UIDrawableComponent>();

				if (uiComponent != nullptr)
				{
					gameObject->Render(renderer);
				}

				if (alphaComponent != nullptr)
					renderer->TurnOffAlphaBlending();
			}
			renderer->TurnZBufferOn();
		}
	}

	void SceneManager::UpdateScene(float DeltaTime)
	{
		if (m_updateEnabled && m_scene)
		{
			for (const auto& object : m_scene->GetStorage())
			{
				object.second.get()->Update(DeltaTime);
			}
		}
	}

	void SceneManager::ToggleGameObjectUpdating()
	{
		m_updateEnabled =! m_updateEnabled;
	}

	inline Scene* SceneManager::GetScene() const
	{
		return m_scene;
	}

	void SceneManager::Clear()
	{
		m_scene->Destroy();
		Core::Utils::Logger::Log("[SceneManager] Current scene deleted");
	}
}