#pragma once
#include "GameObjects/Base/GameObject.h"
namespace S2DE::GameObjects
{
	template<typename T>
	inline T* GameObject::AddComponent(T* component, std::uint32_t priority)
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

	template<typename T>
	inline void GameObject::DeleteComponent()
	{
		static_assert(!std::is_base_of<T, Components::Component>::value || std::is_same<T, Components::Component>::value,
			"This is not Component or Component based class");

		std::type_index type = typeid(T);

		ComponentsList::iterator it = std::find_if(m_components.begin(), m_components.end(), [&type](std::pair<std::pair<boost::uuids::uuid,
			std::type_index>, Components::Component*> const& elem)
			{
				// If we try to get object based on component type
				// Example: We try to get DrawableComponent but that object 
				//			is currently is Sprite(Based on DrawableComponent)
				if (dynamic_cast<T*>(elem.second) != nullptr)
				{
					return true;
				}

				return elem.first.second == type;
			});

		if (it == m_components.end())
		{
			Core::Utils::Logger::Error("%s Can't delete %s component is not found in game object!", GetName().c_str(), typeid(T).name());
			return;
		}

		it->second->OnDestroy();

		m_components.erase(it);
	}

	template<typename T>
	inline T* GameObject::CreateComponent(std::uint32_t priority)
	{
		static_assert(!std::is_base_of<T, Components::Component>::value || std::is_same<T, Components::Component>::value,
			"This is not Component or Component based class");

		auto component = new T();

		component->SetOwner(this);
		component->SetName(Core::Utils::GetClassNameInString(component));

		// When name and owner is setted
		component->OnCreate();

		// TODO: 
		//if(priority != 0)
		//	component->SetPriority();

		m_components.push_back(std::make_pair(std::make_pair(component->GetUUID(), std::type_index(typeid(*component))), component));

		Core::Utils::Logger::Log("[%s] CreateComponent %s", GetName().c_str(), component->GetName().c_str());
		return component;
	}

	template<typename T>
	inline [[nodiscard]] T* GameObject::GetComponent()
	{
		static_assert(!std::is_base_of<T, Components::Component>::value || std::is_same<T, Components::Component>::value,
			"This is not Component or Component based class");

		std::type_index type = typeid(T);

		ComponentsList::iterator it = std::find_if(m_components.begin(), m_components.end(), [&type](std::pair<std::pair<boost::uuids::uuid,
			std::type_index>, Components::Component*> const& elem)
			{
				// If we try to get object based on component type
				// Example: We try to get DrawableComponent but that object 
				//			is currently is Sprite(Based on DrawableComponent)
				if (dynamic_cast<T*>(elem.second) != nullptr)
				{
					return true;
				}

				return elem.first.second == type;
			});

		if (it == m_components.end())
		{
			return nullptr;
		}

		return dynamic_cast<T*>(it->second);
	}

	template<typename T>
	inline [[nodiscard]] T* GameObject::GetComponentInChildren()
	{
		static_assert(!std::is_base_of<T, Components::Component>::value || std::is_same<T, Components::Component>::value,
			"This is not Component or Component based class");

		if (m_transform->GetChild() != nullptr)
		{
			return m_transform->GetChild()->GetComponent<T>();
		}

		return nullptr;
	}

	template<typename T>
	inline [[nodiscard]] T* GameObject::GetComponentInParent()
	{
		static_assert(!std::is_base_of<T, Components::Component>::value || std::is_same<T, Components::Component>::value,
			"This is not Component or Component based class");

		if (m_transform->GetParent() != nullptr)
		{
			return m_transform->GetParent()->GetComponent<T>();
		}

		return nullptr;
	}
}