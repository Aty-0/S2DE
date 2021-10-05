#include "SceneManager.h"
#include "GameObjects/Camera.h"
#include "GameObjects/Sprite.h"

namespace S2DE
{
	SceneManager::SceneManager() : 
		m_scene(nullptr),
		m_update_enabled(true),
		m_render_enabled(true),
		m_render_dbg_gui_enabled(true)
	{

	}

	SceneManager::~SceneManager()
	{
		Delete(m_scene);
	}

	void SceneManager::CreateNewScene()
	{	 
		m_scene = new Scene();
		CreateGameObjectNoInit<Camera>();
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
		for (SceneObjectStorage::iterator it = m_scene->GetStorage().begin(); it != m_scene->GetStorage().end(); it++)
		{
			if(it->second == dynamic_cast<Sprite*>(it->second))
				reinterpret_cast<Sprite*>(it->second)->UpdateShader();
		}
	}

	void SceneManager::UpdateTextures()
	{
		for (SceneObjectStorage::iterator it = m_scene->GetStorage().begin(); it != m_scene->GetStorage().end(); it++)
		{
			if (it->second == dynamic_cast<Sprite*>(it->second))
				reinterpret_cast<Sprite*>(it->second)->UpdateTexture();
		}
	}

	void SceneManager::UpdateInput()
	{
		if (m_update_enabled && m_scene)
		{
			for (SceneObjectStorage::iterator it = m_scene->GetStorage().begin(); it != m_scene->GetStorage().end(); it++)
			{
				it->second->UpdateInput();
			}
		}
	}
		 	 
	void SceneManager::RenderDebugGUI()
	{
		if (m_render_dbg_gui_enabled && m_scene)
		{
			for (SceneObjectStorage::iterator it = m_scene->GetStorage().begin(); it != m_scene->GetStorage().end(); it++)
			{
				it->second->RenderDebugGUI();
			}
		}
	}

	void SceneManager::RenderScene()
	{
		if (m_render_enabled && m_scene)
		{
			for (SceneObjectStorage::iterator it = m_scene->GetStorage().begin(); it != m_scene->GetStorage().end(); it++)
			{
				it->second->Render();
			}
		}
	}

	void SceneManager::UpdateScene(float DeltaTime)
	{
		if (m_update_enabled && m_scene)
		{
			for (SceneObjectStorage::iterator it = m_scene->GetStorage().begin(); it != m_scene->GetStorage().end(); it++)
			{
				it->second->Update(DeltaTime);
			}
		}
	}

	void SceneManager::ToggleGameObjectVisibility()
	{
		m_render_enabled =! m_render_enabled;
	}

	void SceneManager::ToggleDebugGUIVisibility()
	{
		m_render_dbg_gui_enabled = !m_render_dbg_gui_enabled;
	}

	void SceneManager::ToggleGameObjectUpdate()
	{
		m_update_enabled =! m_update_enabled;
	}

	void SceneManager::Clear()
	{
		Delete(m_scene);
		Logger::Log("[SceneManager] Current scene deleted");
	}
}