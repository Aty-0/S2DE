#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/UUID.h"
#include "Base/Utils/Logger.h"
#include "GameObjects/Components/Component.h"
#include "GameObjects/Components/Transform.h"

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
		
		virtual void				 Select() { m_isSelected = true; }
		virtual void				 Unselect() { m_isSelected = false; }


		[[nodiscard]] inline std::string            GetName()   const;
		[[nodiscard]] inline std::int32_t			GetPrefix() const;
		[[nodiscard]] inline std::string            GetType()   const;
		[[nodiscard]] inline bool                   isVisible() const;
		[[nodiscard]] inline bool                   isEnabled() const;
		[[nodiscard]] inline bool                   isSelected() const;
		[[nodiscard]] inline Components::Transform* GetTransform() const;
		[[nodiscard]] inline ComponentsList			GetComponentsList() const;
	private:
		std::string					 m_name; 
		std::int32_t				 m_prefix;
		std::string					 m_type;
		bool						 m_enabled;
		bool						 m_visible;
		bool						 m_isSelected;
		Components::Transform*		 m_transform;
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

			m_components.push_back(std::make_pair(std::make_pair(component->GetUUID(), std::type_index(typeid(*component))), component));

			return component;
		}

		template<typename T = Components::Component>
		T* GetComponent() 
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
		T* GetComponentInChildren() 
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
		T* GetComponentInParent() 
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
		ComponentsList m_components;

		// FIX ME: Not work for some reason 
		// S2DE_SERIALIZE_BEGIN(S2DE::Core::Utils::UUID);

		S2DE_SERIALIZE_BASE_BEGIN();
		S2DE_SERIALIZE_ADD(m_name);
		S2DE_SERIALIZE_ADD(m_prefix);
		S2DE_SERIALIZE_ADD(m_type);
		S2DE_SERIALIZE_ADD(m_enabled);
		S2DE_SERIALIZE_ADD(m_visible);
		S2DE_SERIALIZE_ADD(m_isSelected);
		// FIX ME:
		S2DE_SERIALIZE_ADD(m_uuidInStringFIXME);
		S2DE_SERIALIZE_ADD(m_transform);
		//S2DE_SERIALIZE_ADD(m_components);
		S2DE_SERIALIZE_END();
	};
}

S2DE_SERIALIZE_CLASS_IMPLEMENTATION(S2DE::GameObjects::GameObject, boost::serialization::object_serializable);
S2DE_SERIALIZE_CLASS_IMPLEMENTATION(S2DE::GameObjects::ComponentsList, boost::serialization::object_serializable);