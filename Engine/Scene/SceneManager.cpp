#include "SceneManager.h"
#include "GameObjects/Camera.h"
#include "GameObjects/Drawable.h"

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
		CreateGameObject<GameObjects::Camera>(S2DE_MAIN_CAMERA_NAME, S2DE_ENGINE_GAMEOBJECT_TYPE, -1);
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
			if(object.second.get() == dynamic_cast<GameObjects::Drawable*>(object.second.get()))
				reinterpret_cast<GameObjects::Drawable*>(object.second.get())->UpdateShader();
	}

	void SceneManager::UpdateTextures()
	{
		for (const auto& object : m_scene->GetStorage())
			if (object.second.get() == dynamic_cast<GameObjects::Drawable*>(object.second.get()))
				reinterpret_cast<GameObjects::Drawable*>(object.second.get())->UpdateTexture();	
	}

	void SceneManager::UpdateInput()
	{
		if (m_update_enabled && m_scene)
			for (const auto& object : m_scene->GetStorage())
				object.second.get()->UpdateInput();
	}
		 	 
	void SceneManager::RenderImGUI()
	{
		if (m_render_imgui_enabled && m_scene)
			for (const auto& object : m_scene->GetStorage())
				object.second.get()->RenderImGUI();

	}

	void SceneManager::RenderScene()
	{
		if (m_render_enabled && m_scene)
			for (const auto& object : m_scene->GetStorage())
				object.second.get()->Render();

	}

	void SceneManager::UpdateScene(float DeltaTime)
	{
		if (m_update_enabled && m_scene)
			for (const auto& object : m_scene->GetStorage())
				object.second.get()->Update(DeltaTime);
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