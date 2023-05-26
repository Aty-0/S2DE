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

	// FIX ME:  Reducing perfomance
	//			Need to make better algorithm, currently it is very bad way to do this.

	std::string Scene::MakeNameUnique(std::string name)
	{
		auto newName = std::string();

		// How much objects we found with the same name
		auto count = 0;

		SceneObjectStorage::iterator it = { };
		while (true)
		{
			count != 0 ? newName = name + std::to_string(count) : newName = name;
			it = std::find_if(
				m_storage.begin(), m_storage.end(),
				[&newName](std::pair<std::pair<std::string, boost::uuids::uuid>,
					std::shared_ptr<GameObjects::GameObject>> const& elem) {
						return elem.first.first == newName;
				});

			if (it == m_storage.end())
			{
				break;
			}

			count++;
		}

		return newName;
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
			Core::Utils::Logger::Error("[Scene] [%s] Can't delete object Name: %s",
				m_name.c_str(), 
				object_name.c_str());
			return;
		}
		
		Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0.7f, 0.4f, 0.8f, 1.0f), "[Scene] [%s] Delete [%s] Name: %s UUID: %s",
			m_name.c_str(), 
			Core::Utils::GetClassNameInString(it->second.get()).c_str(),
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
			Core::Utils::Logger::Error("[Scene] [%s] Can't delete object UUID: %s",
				m_name.c_str(), 
				Core::Utils::UUID::ConvertUUIDToString(object_id).c_str());
			return;
		}

		Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0.7f, 0.4f, 0.8f, 1.0f), "[Scene] [%s] Delete [%s] Name: %s UUID: %s",
			m_name.c_str(), 
			Core::Utils::GetClassNameInString(it->second.get()).c_str(),
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
		Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0.7f, 0.4f, 0.8f, 1.0f), "[Scene] [%s] Try to clear scene...", m_name.c_str());

		for (SceneObjectStorage::iterator it = m_storage.begin(); it != m_storage.end();)
		{
			if (it->second->GetPrefix() != -1 || it->second->GetType() != S2DE_ENGINE_GAMEOBJECT_TYPE)
			{
				Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0.7f, 0.4f, 0.8f, 1.0f), "[Scene] [%s] Delete [%s] Name: %s UUID: %s",
					m_name.c_str(), 
					Core::Utils::GetClassNameInString(it->second.get()).c_str(),
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
		Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0.7f, 0.4f, 0.8f, 1.0f), "[Scene] [%s] Destroy ", m_name.c_str());
		m_storage.erase(m_storage.begin(), m_storage.end());
		m_storage.shrink_to_fit();
	}

	inline SceneObjectStorage& Scene::GetStorage()
	{
		return m_storage;
	}

	inline std::string Scene::GetName() const
	{
		return m_name;
	}

}