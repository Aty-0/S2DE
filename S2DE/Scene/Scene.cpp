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


	//FIX ME 
	//Reduces perfomance
	void Scene::CheckNameOnExist(std::string& name)
	{
		std::uint32_t count = 0;
		std::string new_name = std::string();

		while (true)
		{
			//If count == 0 then we set the name of the original 
			//if we found object we set name with count
			count != 0 ? new_name = name + std::to_string(count) : new_name = name;
			SceneObjectStorage::iterator it = std::find_if(
				m_storage.begin(), m_storage.end(),
				[&new_name](std::pair<std::pair<std::string, boost::uuids::uuid>,
					GameObject*> const& elem) {
						return elem.first.first == new_name;
				});

			//if we not found object
			if (it == std::end(m_storage))
			{
				name = new_name;
				break;
			}

			count++;
		}
	}

	void Scene::Add(GameObject* g)
	{
		//Check object on valid
		if (g == nullptr)
		{
			Logger::Error("[Scene] [%s] Can't add this game object because it is nullptr!", m_name.c_str());
			return;
		}
		else if (isStringEmpty(g->GetName()) || isStringEmpty(g->GetID()))
		{
			Logger::Error("[Scene] [%s] Can't add this game object because it is not initialized!", m_name.c_str());
			return;
		}

		std::string name = g->GetName();

		CheckNameOnExist(name);
		g->SetName(name);

		Logger::Log("[Scene] [%s] Added name: %s id: %s", m_name.c_str(), name.c_str(), g->GetID().c_str());

		//Add object to storage
		m_storage.push_back(std::make_pair(std::make_pair(name, g->GetUUID()), g));
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
		Logger::Log("[Scene] [%s] Delete object %s", m_name.c_str(), object_name.c_str());

		SceneObjectStorage::iterator it = std::remove_if(m_storage.begin(), m_storage.end(),
			[&object_name](std::pair<std::pair<std::string, boost::uuids::uuid>, GameObject*> const& elem) {
				return elem.first.first == object_name;
			});

		if (it == m_storage.end() || it->second->GetPrefix() == -1)
		{
			Logger::Error("[Scene] [%s] Can't delete object %s", m_name.c_str(), object_name.c_str());
			return;
		}

		m_storage.erase(it, m_storage.end());
	}	 
		 
	void Scene::Delete(boost::uuids::uuid object_id)
	{	 
		Logger::Log("[Scene] [%s] Delete object %s", m_name.c_str(), GameObjectIDGenerator::ConvertUUIDToString(object_id).c_str());

		SceneObjectStorage::iterator it = std::remove_if(m_storage.begin(), m_storage.end(),
			[&object_id](std::pair<std::pair<std::string, boost::uuids::uuid>, GameObject*> const& elem) {
				return elem.first.second == object_id;
			});

		if (it == m_storage.end() || it->second->GetPrefix() == -1)
		{
			Logger::Error("[Scene] [%s] Can't delete object %s", m_name.c_str(), GameObjectIDGenerator::ConvertUUIDToString(object_id).c_str());
			return;
		}

		m_storage.erase(it, m_storage.end());
		m_storage.shrink_to_fit();
	}	 
		 
	void Scene::Replace(std::string object_name, GameObject* object)
	{	 
		S2DE_NO_IMPL();
	}	 
		 
	void Scene::Replace(boost::uuids::uuid object_id, GameObject* object)
	{	 
		S2DE_NO_IMPL();
	}	 

	void Scene::Clear()
	{
		Logger::Log("[Scene] [%s] Try to clear scene...", m_name.c_str());

		for (SceneObjectStorage::iterator it = m_storage.begin(); it != m_storage.end();)
		{
			if (it->second->GetPrefix() != -1)
			{
				S2DE::Delete(it->second);
				it = m_storage.erase(it);
			}
			else
				it++;
		}

		m_storage.shrink_to_fit();
	}

	void Scene::Destroy()
	{
		Logger::Log("[Scene] [%s] Destroy ", m_name.c_str());
		m_storage.erase(m_storage.begin(), m_storage.end());
		m_storage.shrink_to_fit();
	}
}