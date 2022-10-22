#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/UUID.h"
#include "Base/Utils/Logger.h"

#include "GameObjects/Components/Component.h"
#include "GameObjects/Components/Transform.h"

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>


//TODO: 1. Prefix 
//		2. Objects serialization

#include <typeinfo>
#include <typeindex>

#define S2DE_UUID_REGENERATE "R"
#define S2DE_DEFAULT_GAMEOBJECT_NAME "GameObject"
#define S2DE_DEFAULT_GAMEOBJECT_TYPE "Default"
#define S2DE_DEFAULT_GAMEOBJECT_PREFIX 0
#define	S2DE_ENGINE_GAMEOBJECT_TYPE "_Engine_"


namespace S2DE::GameObjects
{
	typedef std::vector<std::pair<std::pair<boost::uuids::uuid, std::type_index>, Components::Component*>> ComponentsList;
	class S2DE_API GameObject : public Core::Utils::UUID
	{
	public:
		GameObject();
		explicit GameObject(std::string name, std::string type, std::int32_t prefix, std::string id = std::string());

		virtual ~GameObject();

		
		void						 Render();
		void						 Update(float deltaTime);
		void						 SetName(std::string name);
		void						 SetPrefix(std::int32_t prefix);
		void						 SetType(std::string type);
		void						 SetVisible(bool visible);
		void						 SetEnabled(bool enabled);
									 
		inline std::string           GetName()   const { return m_name; }
		inline std::int32_t			 GetPrefix() const { return m_prefix; }
		inline std::string           GetType()   const { return m_type; }
		inline bool                  isVisible() const { return m_visible; }
		inline bool                  isEnabled() const { return m_enabled; }
		inline bool                  isSelected() const { return m_isSelected; }
		
		virtual void				 Select() { m_isSelected = true; }
		virtual void				 Unselect() { m_isSelected = false; }

		// Get transform component
		inline Components::Transform*			 GetTransform() const { return m_transform; }
		inline ComponentsList		GetComponentsList() const { return m_components; }
	private:
		std::string					 m_name; 
		std::int32_t				 m_prefix;
		std::string					 m_type;
		bool						 m_enabled;
		bool						 m_visible;
		bool						 m_isSelected;

		Components::Transform*					 m_transform;
	public:
		template<typename T>
		T* AddComponent(T* component, std::uint32_t priority = 0)
		{
			static_assert(!std::is_base_of<T, Components::Component>::value || std::is_same<T, Components::Component>::value,
				"This is not Component or Component based class");

			component->OnCreate();
			component->SetOwner(this);
			component->SetName(Core::Utils::GetClassNameInString(component));
			Core::Utils::Logger::Log("[%s] AddComponent %s", GetName().c_str(), component->GetName().c_str());

			m_components.push_back(std::make_pair(std::make_pair(component->GetUUID(), std::type_index(typeid(T))), component));
			return component;
		}

		// TODO: delete by priority ?

		template<typename T = Components::Component>
		void DeleteComponent()
		{
			static_assert(!std::is_base_of<T, Components::Component>::value || std::is_same<T, Components::Component>::value,
				"This is not Component or Component based class");

			auto component = GetComponent<T>();
			component->OnDestroy();

			m_components.erase(component);
		}

		template<typename T = Components::Component>
		T* CreateComponent(std::uint32_t priority = 0)
		{
			static_assert(!std::is_base_of<T, Components::Component>::value || std::is_same<T, Components::Component>::value,
				"This is not Component or Component based class");

			auto component = new T();

			component->SetOwner(this);
			component->SetName(Core::Utils::GetClassNameInString(component));
			Core::Utils::Logger::Log("[%s] CreateComponent %s", GetName().c_str(), component->GetName().c_str());
			// When name and owner is setted
			component->OnCreate();

			// TODO: 
			//if(priority != 0)
			//	component->SetPriority();

			m_components.push_back(std::make_pair(std::make_pair(component->GetUUID(), std::type_index(typeid(T))), component));

			return component;
		}

		template<typename T = Components::Component>
		inline T* GetComponent() 
		{ 
			static_assert(!std::is_base_of<T, Components::Component>::value || std::is_same<T, Components::Component>::value,
				"This is not Component or Component based class");

			std::type_index typeIndex = std::type_index(typeid(T));

			ComponentsList::iterator it = std::find_if(m_components.begin(), m_components.end(), [&typeIndex](std::pair<std::pair<boost::uuids::uuid,
					std::type_index>, Components::Component*> const& elem)
			{ 
				return elem.first.second == typeIndex;
			});

			// For avoid null casting 
			if (it == m_components.end())
				return nullptr;

			return dynamic_cast<T*>(it->second);
		}

		template<typename T = Components::Component>
		inline T* GetComponentInChildren() 
		{
			static_assert(!std::is_base_of<T, Components::Component>::value || std::is_same<T, Components::Component>::value,
				"This is not Component or Component based class");

			if (m_transform->GetChild() != nullptr)
			{
				return m_transform->GetChild()->GetComponent<T>();
			}

			return nullptr;
		}

		template<typename T = Components::Component>
		inline T* GetComponentInParent() 
		{
			static_assert(!std::is_base_of<T, Components::Component>::value || std::is_same<T, Components::Component>::value,
				"This is not Component or Component based class");

			if (m_transform->GetParent() != nullptr)
			{
				return m_transform->GetParent()->GetComponent<T>();
			}

			return nullptr; 
		}

	private:
		// TODO: smart pointer
		ComponentsList m_components;


	};
}