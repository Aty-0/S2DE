#include "SceneManager.h"
#include "GameObjects/Camera.h"

namespace S2DE
{
	SceneManager::SceneManager()
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

	void SceneManager::UpdateInput()
	{
		for (SceneObjectStorage::iterator it = m_scene->GetStorage().begin(); it != m_scene->GetStorage().end(); it++)
		{
			it->second->UpdateInput();
		}
	}
		 	 
	void SceneManager::RenderDebugGUI()
	{
		for (SceneObjectStorage::iterator it = m_scene->GetStorage().begin(); it != m_scene->GetStorage().end(); it++)
		{
			it->second->RenderDebugGUI();
		}
	}

	void SceneManager::RenderScene()
	{
		for (SceneObjectStorage::iterator it = m_scene->GetStorage().begin(); it != m_scene->GetStorage().end(); it++)
		{
			it->second->Render();
		}
	}

	void SceneManager::UpdateScene(float DeltaTime)
	{
		for (SceneObjectStorage::iterator it = m_scene->GetStorage().begin(); it != m_scene->GetStorage().end(); it++)
		{
			it->second->Update(DeltaTime);
		}
	}

	void SceneManager::Clear()
	{

	}
}