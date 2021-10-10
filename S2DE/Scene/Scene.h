#pragma once
#include "Base/Main/Common.h"
#include "Base/Engine.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/GameObjectIDGenerator.h"

namespace S2DE
{
	//SceneObjectStorage type
	typedef std::vector<std::pair<std::pair<std::string, boost::uuids::uuid>, 
		GameObject*>> SceneObjectStorage;

	class S2DE_API Scene
	{
	public:
		Scene();
		~Scene();
		
		//Rename object (by name)
		void							Rename(std::string object_name, std::string new_object_name);
		//Rename object (by id)
		void							Rename(boost::uuids::uuid object_id, std::string new_object_name);

		//Delete game object (by name)
		void							Delete(std::string object_name);
		//Delete game object (by id)
		void							Delete(boost::uuids::uuid object_id);

		//Replace game object (by name)
		void							Replace(std::string object_name, GameObject* object);
		//Replace game object (by id)
		void							Replace(boost::uuids::uuid object_id, GameObject* object);

		//Clear all scene
		//NOTE: Object with engine prefix (-1) will not been deleted
		void							Clear();
		//Destroy scene
		void							Destroy();

		//Add game object to scene
		void							Add(GameObject* g);

		//Check object name on exist
		//if name existing we add number of same object
		void							CheckNameOnExist(std::string& name);

		inline SceneObjectStorage&		GetStorage() { return m_storage; }
		inline std::string				GetName() const { return m_name; }

		//Clone game object (by name)
		template<typename T>
		void							Clone(std::string object_name, std::string new_name = std::string())
		{
			static_assert(!std::is_base_of<T, GameObject>::value, "This class not based on GameObject class");

			Logger::Log("[Scene] [%s] Clone object Name: %s New name: %s", m_name.c_str(), object_name.c_str(),
				isStringEmpty(new_name) ? "No" : new_name.c_str());

			SceneObjectStorage::iterator it = std::find_if(m_storage.begin(), m_storage.end(),
				[&object_name](std::pair<std::pair<std::string, boost::uuids::uuid>, GameObject*> const& elem)
				{ return elem.first.first == object_name; });

			if (it == m_storage.end())
			{
				Logger::Error("[Scene] [%s] Can't clone object!", m_name.c_str());
				return;
			}
			
			if (dynamic_cast<T*>(it->second) != it->second)
			{
				Logger::Error("[Scene] [%s] Wrong type! Can't clone object", m_name.c_str());
				return;
			}

			GameObject* new_obj = new T(*dynamic_cast<T*>(it->second));

			std::string name = isStringEmpty(new_name) ? new_obj->GetName() + "_clone" : new_name;
			new_obj->Init(name, new_obj->GetType(), new_obj->GetPrefix(), "REGENERATE");

			Add(new_obj);
		}

		//Clone game object (by id)
		template<typename T>
		void							Clone(boost::uuids::uuid object_id, std::string new_name = std::string())
		{
			static_assert(!std::is_base_of<T, GameObject>::value, "This class not based on GameObject class");

			Logger::Log("[Scene] [%s] Clone object Name: %s New name: %s", m_name.c_str(), GameObjectIDGenerator::ConvertUUIDToString(object_id).c_str(),
				isStringEmpty(new_name) ? "No" : new_name.c_str());

			SceneObjectStorage::iterator it = std::find_if(m_storage.begin(), m_storage.end(),
				[&object_id](std::pair<std::pair<std::string, boost::uuids::uuid>, GameObject*> const& elem)
				{ return elem.first.second == object_id; });

			if (it == m_storage.end())
			{
				Logger::Error("[Scene] [%s] Can't clone object!", m_name.c_str());
				return;
			}

			if (dynamic_cast<T*>(it->second) != it->second)
			{
				Logger::Error("[Scene] [%s] Wrong type! Can't clone object", m_name.c_str());
				return;
			}

			GameObject* new_obj = new T(*dynamic_cast<T*>(it->second));

			std::string name = isStringEmpty(new_name) ? new_obj->GetName() + "_clone" : new_name;
			new_obj->Init(name, new_obj->GetType(), new_obj->GetPrefix(), "REGENERATE");

			Add(new_obj);
		}

	private:
		SceneObjectStorage m_storage;
		std::string m_name;
	};
}
