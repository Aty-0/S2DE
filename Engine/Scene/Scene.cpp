#include "Scene.h"

namespace S2DE::Scene
{
	Scene::Scene() : 
		m_name("Untitled"),
		m_storage()
	{

	}

	Scene::~Scene()
	{

	}

	//FIX ME: Reducing perfomance
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
					std::shared_ptr<GameObjects::GameObject>> const& elem) {
						return elem.first.first == new_name;
				});

			//if we not found object
			if (it == m_storage.end())
			{
				name = new_name;
				break;
			}

			count++;
		}
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
		SceneObjectStorage::iterator it = std::find_if(
			m_storage.begin(),
			m_storage.end(),
			[&object_name](std::pair<std::pair<std::string, boost::uuids::uuid>,
				std::shared_ptr<GameObjects::GameObject>> const& elem) {
					return elem.first.first == object_name;
			});
		
		if (it == m_storage.end() || it->second->GetPrefix() == -1 || it->second->GetType() == S2DE_ENGINE_GAMEOBJECT_TYPE)
		{
			Logger::Error("[Scene] [%s] Can't delete object Name: %s",
				m_name.c_str(), 
				object_name.c_str());
			return;
		}
		
		Logger::Log("[Scene] [%s] Delete [%s] Name: %s UUID: %s", 
			m_name.c_str(), 
			Core::Other::GetClassNameInString(it->second.get()).c_str(), 
			it->second.get()->GetName().c_str(), 
			it->second.get()->GetUUIDString().c_str());
		
		m_storage.erase(it);
	}	 
		 
	void Scene::Delete(boost::uuids::uuid object_id)
	{	 
		SceneObjectStorage::iterator it = std::find_if(
			m_storage.begin(), 
			m_storage.end(),
			[&object_id](std::pair<std::pair<std::string, boost::uuids::uuid>, 
				std::shared_ptr<GameObjects::GameObject>> const& elem) {
				return elem.first.second == object_id;
			});

		if (it == m_storage.end() || it->second->GetPrefix() == -1 || it->second->GetType() == S2DE_ENGINE_GAMEOBJECT_TYPE)
		{
			Logger::Error("[Scene] [%s] Can't delete object UUID: %s", 
				m_name.c_str(), 
				GameObjects::GameObjectIDGenerator::ConvertUUIDToString(object_id).c_str());
			return;
		}

		Logger::Log("[Scene] [%s] Delete [%s] Name: %s UUID: %s", 
			m_name.c_str(), 
			Core::Other::GetClassNameInString(it->second.get()).c_str(), 
			it->second.get()->GetName().c_str(),
			it->second.get()->GetUUIDString().c_str());

		m_storage.erase(it);
	}	 
		 
	void Scene::Replace(std::string object_name, GameObjects::GameObject* object)
	{	 
		S2DE_NO_IMPL();
	}	 
		 
	void Scene::Replace(boost::uuids::uuid object_id, GameObjects::GameObject* object)
	{	 
		S2DE_NO_IMPL();
	}	 

	void Scene::Clear()
	{
		Logger::Log("[Scene] [%s] Try to clear scene...", m_name.c_str());

		for (SceneObjectStorage::iterator it = m_storage.begin(); it != m_storage.end();)
		{
			if (it->second->GetPrefix() != -1 || it->second->GetType() != S2DE_ENGINE_GAMEOBJECT_TYPE)
			{
				Logger::Log("[Scene] [%s] Delete [%s] Name: %s UUID: %s", 
					m_name.c_str(), 
					Core::Other::GetClassNameInString(it->second.get()).c_str(),
					it->second.get()->GetName().c_str(),
					it->second.get()->GetUUIDString().c_str());

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