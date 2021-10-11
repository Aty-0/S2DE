#pragma once
#include "Base/Main/Common.h"
#include "Base/Engine.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/GameObjectIDGenerator.h"

namespace S2DE
{
	//SceneObjectStorage type
	typedef std::vector<std::pair<std::pair<std::string, boost::uuids::uuid>, std::shared_ptr<GameObject>>> SceneObjectStorage;

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

		//Check object name on exist
		//if name existing we add number of same object
		void							CheckNameOnExist(std::string& name);

		inline SceneObjectStorage&		GetStorage() { return m_storage; }
		inline std::string				GetName() const { return m_name; }

		//Add game object to scene
		template<typename T>
		void							Add(T* g)
		{
			static_assert(!std::is_base_of<T, GameObject>::value, "This is not GameObject based class");

			//Check object on valid
			if (g == nullptr)
			{
				Logger::Error("[Scene] [%s] Can't add this game object because it is nullptr!", m_name.c_str());
				return;
			}
			else if (isStringEmpty(g->GetName()) || isStringEmpty(g->GetUUIDString()))
			{
				Logger::Error("[Scene] [%s] Can't add this game object because it is not initialized!", m_name.c_str());
				return;
			}

			std::string name = g->GetName();

			CheckNameOnExist(name);
			g->SetName(name);

			Logger::Log("[Scene] [%s] Added %s Name: %s UUID: %s", m_name.c_str(), typeid(g).name() ,name.c_str(), g->GetUUIDString().c_str());

			//Add object to storage
			m_storage.push_back(std::make_pair(std::make_pair(name, g->GetUUID()), std::make_shared<T>(*g)));
		}


		//Clone game object (by name)
		template<typename T>
		void							Clone(std::string object_name, std::string new_name = std::string())
		{
			static_assert(!std::is_base_of<T, GameObject>::value, "This is not GameObject based class");

			Logger::Log("[Scene] [%s] Clone object Name: %s New name: %s", m_name.c_str(), object_name.c_str(),
				isStringEmpty(new_name) ? "No" : new_name.c_str());

			SceneObjectStorage::iterator it = std::find_if(m_storage.begin(), m_storage.end(),
				[&object_name](std::pair<std::pair<std::string, boost::uuids::uuid>, std::shared_ptr<GameObject>> const& elem)
				{ return elem.first.first == object_name; });

			if (it == m_storage.end())
			{
				Logger::Error("[Scene] [%s] Can't clone object!", m_name.c_str());
				return;
			}
			
			if (dynamic_cast<T*>(it->second.get()) != it->second.get())
			{
				Logger::Error("[Scene] [%s] Wrong type! Can't clone object", m_name.c_str());
				return;
			}

			auto new_obj = new T(*dynamic_cast<T*>(it->second.get()));

			std::string name = isStringEmpty(new_name) ? new_obj->GetName() + "_clone" : new_name;
			new_obj->Init(name, new_obj->GetType(), new_obj->GetPrefix(), "REGENERATE");

			Add<T>(new_obj);
		}

		//Clone game object (by id)
		template<typename T>
		void							Clone(boost::uuids::uuid object_id, std::string new_name = std::string())
		{
			static_assert(!std::is_base_of<T, GameObject>::value, "This is not GameObject based class");

			Logger::Log("[Scene] [%s] Clone object Name: %s New name: %s", m_name.c_str(), GameObjectIDGenerator::ConvertUUIDToString(object_id).c_str(),
				isStringEmpty(new_name) ? "No" : new_name.c_str());

			SceneObjectStorage::iterator it = std::find_if(m_storage.begin(), m_storage.end(),
				[&object_id](std::pair<std::pair<std::string, boost::uuids::uuid>, std::shared_ptr<GameObject>> const& elem)
				{ return elem.first.second == object_id; });

			if (it == m_storage.end())
			{
				Logger::Error("[Scene] [%s] Can't clone object!", m_name.c_str());
				return;
			}

			if (dynamic_cast<T*>(it->second.get()) != it->second.get())
			{
				Logger::Error("[Scene] [%s] Wrong type! Can't clone object", m_name.c_str());
				return;
			}

			auto new_obj = new T(*dynamic_cast<T*>(it->second));

			std::string name = isStringEmpty(new_name) ? new_obj->GetName() + "_clone" : new_name;
			new_obj->Init(name, new_obj->GetType(), new_obj->GetPrefix(), "REGENERATE");

			Add<T>(new_obj);
		}

	private:
		SceneObjectStorage m_storage;
		std::string m_name;
	};
}
