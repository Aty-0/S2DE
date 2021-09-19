#include "Scene.h"

namespace S2DE
{
	Scene::Scene() : 
		m_name("Untitled"),
		m_storage()
	{

	}

	Scene::~Scene()
	{
		Destroy();
	}
 
	void Scene::Add(GameObject* g)
	{
		//Check object on valid
		if (g == nullptr)
		{
			Logger::Error("[Scene] Can't add this game object because it is nullptr!");
			return;
		}
		else if (isStringEmpty(g->GetName()) || isStringEmpty(g->GetID()))
		{
			Logger::Error("[Scene] Can't add this game object because it is not initialized!");
			return;
		}

		Logger::Log("[Scene] Add %s %s", g->GetName().c_str(), g->GetID().c_str());

		//Add object to storage
		m_storage.push_back(std::make_pair(std::make_pair(g->GetName(), g->GetUUID()), g));
	}

	void Scene::Rename(std::string object_name, std::string new_object_name)
	{	 
		S2DE_NO_IMPL();
	}	 
		 
	void Scene::Rename(boost::uuids::uuid object_id, std::string new_object_name)
	{	 
		S2DE_NO_IMPL();
	}	 
		 
	void Scene::Delete(std::string object_name)
	{	 
		S2DE_NO_IMPL();
	}	 
		 
	void Scene::Delete(boost::uuids::uuid object_id)
	{	 
		S2DE_NO_IMPL();
	}	 
		 
	void Scene::Replace(std::string object_name, GameObject* object)
	{	 
		S2DE_NO_IMPL();
	}	 
		 
	void Scene::Replace(boost::uuids::uuid object_id, GameObject* object)
	{	 
		S2DE_NO_IMPL();
	}	 
		 
	void Scene::Clone(std::string object_name)
	{	 
		S2DE_NO_IMPL();
	}	 
		 
	void Scene::Clone(boost::uuids::uuid object_id)
	{
		S2DE_NO_IMPL();
	}

	void Scene::Clear()
	{
		S2DE_NO_IMPL();
	}

	void Scene::Destroy()
	{
		S2DE_NO_IMPL();
	}
}