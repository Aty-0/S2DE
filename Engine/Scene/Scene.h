#pragma once
#include "Base/Main/Common.h"
#include "Base/Engine.h"
#include "GameObjects/Base/GameObject.h"
#include "Base/Utils/UUID.h"

namespace S2DE::Scene
{
	typedef std::vector<std::pair<std::pair<std::string, boost::uuids::uuid>, 
		std::shared_ptr<GameObjects::GameObject>>> SceneObjectStorage;

	class S2DE_API Scene
	{
	public:
		Scene();
		~Scene();
		
		// Rename object (by name)
		void							Rename(std::string object_name, std::string new_object_name);
		// Rename object (by uuid)
		void							Rename(boost::uuids::uuid object_id, std::string new_object_name);

		// Delete game object (by name)
		void							Delete(std::string object_name);
		// Delete game object (by uuid)
		void							Delete(boost::uuids::uuid object_id);

		// Replace game object (by name)
		void							Replace(std::string object_name, GameObjects::GameObject* object);
		// Replace game object (by uuid)
		void							Replace(boost::uuids::uuid object_id, GameObjects::GameObject* object);

		// Clear scene 
		// NOTE: Objects with engine prefix (it is -1) can't be removed
		void							Clear();

		// Destroy scene, remove all objects
		void							Destroy();

		// Check name on uniqueness, if we found object with same name we are add num prefix 
		std::string						MakeNameUnique(std::string name);

		[[nodiscard]] inline SceneObjectStorage& GetStorage();
		[[nodiscard]] inline std::string				GetName() const;

		// Add game object to scene
		template<typename T>
		T*							Add(T* g)
		{
			static_assert(!std::is_base_of<T, GameObjects::GameObject>::value || std::is_same<T, GameObjects::GameObject>::value, "This is not GameObject or GameObject based class");

			// Check object on valid
			if (g == nullptr)
			{
				Core::Utils::Logger::Error("[Scene] [%s] Can't add this game object because it is nullptr!", m_name.c_str());
				return nullptr;
			}
			else if (Core::Utils::isStringEmpty(g->GetName()) || Core::Utils::isStringEmpty(g->GetUUIDString()))
			{
				Core::Utils::Logger::Error("[Scene] [%s] Can't add this game object because it is not initialized!", m_name.c_str());
				return nullptr;
			}

			// Check objects on the same name 
			g->SetName(MakeNameUnique(g->GetName()));

			Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0.7f, 0.4f, 0.8f, 1.0f), "[Scene] [%s] Added [%s] Name: %s UUID: %s at Position (%f %f %f)",
				m_name.c_str(), Core::Utils::GetClassNameInString(g).c_str(),
				g->GetName().c_str(), g->GetUUIDString().c_str(),
				g->GetTransform()->GetPosition().x, g->GetTransform()->GetPosition().y, g->GetTransform()->GetPosition().z);

			// Add object to storage
			const auto gameObjectPtr = std::make_shared<T>(*g);
			m_storage.push_back(std::move(std::make_pair(std::make_pair(g->GetName(), g->GetUUID()), gameObjectPtr)));

			return gameObjectPtr.get();
		}


		// Clone game object (by name)
		template<typename T>
		T* Clone(std::string object_name, std::string new_name = std::string())
		{
			static_assert(!std::is_base_of<T, GameObjects::GameObject>::value || std::is_same<T, GameObjects::GameObject>::value, "This is not GameObject or GameObject based class");

			Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0.7f, 0.4f, 0.8f, 1.0f), "[Scene] [%s] Clone object Name: %s New name: %s", m_name.c_str(), object_name.c_str(),
				Core::Utils::isStringEmpty(new_name) ? "No" : new_name.c_str());

			SceneObjectStorage::iterator it = std::find_if(m_storage.begin(), m_storage.end(),
				[&object_name](std::pair<std::pair<std::string, boost::uuids::uuid>, std::shared_ptr<GameObjects::GameObject>> const& elem)
				{ return elem.first.first == object_name; });

			if (it == m_storage.end())
			{
				Core::Utils::Logger::Error("[Scene] [%s] Can't clone object!", m_name.c_str());
				return nullptr;
			}
			
			if (dynamic_cast<T*>(it->second.get()) != it->second.get())
			{
				Core::Utils::Logger::Error("[Scene] [%s] Wrong type! Can't clone object", m_name.c_str());
				return nullptr;
			}

			const auto new_obj = new T(*dynamic_cast<T*>(it->second.get()));
			const auto name = Core::Utils::isStringEmpty(new_name) ? new_obj->GetName() + "_clone" : new_name;
			new_obj->Init(name, new_obj->GetType(), new_obj->GetPrefix(), "REGENERATE");

			return Add<T>(new_obj);
		}

		// Clone game object (by id)
		template<typename T>
		T* Clone(boost::uuids::uuid object_id, std::string new_name = std::string())
		{
			static_assert(!std::is_base_of<T, GameObjects::GameObject>::value || std::is_same<T, GameObjects::GameObject>::value, "This is not GameObject or GameObject based class");

			Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0.7f, 0.4f, 0.8f, 1.0f), "[Scene] [%s] Clone object Name: %s New name: %s", m_name.c_str(), Core::Utils::UUID::ConvertUUIDToString(object_id).c_str(),
				Core::Utils::isStringEmpty(new_name) ? "No" : new_name.c_str());

			SceneObjectStorage::iterator it = std::find_if(m_storage.begin(), m_storage.end(),
				[&object_id](std::pair<std::pair<std::string, boost::uuids::uuid>, std::shared_ptr<GameObjects::GameObject>> const& elem)
				{ return elem.first.second == object_id; });

			if (it == m_storage.end())
			{
				Core::Utils::Logger::Error("[Scene] [%s] Can't clone object!", m_name.c_str());
				return nullptr;
			}

			if (dynamic_cast<T*>(it->second.get()) != it->second.get())
			{
				Core::Utils::Logger::Error("[Scene] [%s] Wrong type! Can't clone object", m_name.c_str());
				return nullptr;
			}

			const auto new_obj = new T(*dynamic_cast<T*>(it->second));
			const auto name = Core::Utils::isStringEmpty(new_name) ? new_obj->GetName() + "_clone" : new_name;
			new_obj->Init(name, new_obj->GetType(), new_obj->GetPrefix(), "REGENERATE");

			return Add<T>(new_obj);
		}

	private:
		SceneObjectStorage m_storage;
		std::string m_name;
	};
}
