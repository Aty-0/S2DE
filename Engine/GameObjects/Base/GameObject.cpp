#include "GameObject.h"

namespace S2DE::GameObjects
{
	GameObject::GameObject() :
		m_name(std::string()),
		m_type(std::string()),
		m_prefix(0),
		m_visible(true),
		m_enabled(true),
		m_isSelected(false)
	{								  
	}

	GameObject::GameObject(std::string name, std::string type, std::int32_t prefix, std::string id) :
		m_name(std::string()),
		m_type(std::string()),
		m_prefix(0),
		m_visible(true),
		m_enabled(true),
		m_isSelected(false)
	{
		// Basic setup: set name, type and prefix 
		SetName(name);
		SetType(type);
		SetPrefix(prefix);

		// TODO: Need check UUID on exist or invalid value
		if (id == S2DE_UUID_REGENERATE)
			RegenerateUUID();
		else if (!Core::Utils::isStringEmpty(id))
			SetUUID(id);


		//OnCreate();

		m_transform = CreateComponent<Components::Transform>();
	}

	GameObject::~GameObject()
	{
		//OnDestroy();

		m_transform = nullptr;
		m_components.clear();
		m_type.clear();
		m_name.clear();
		m_prefix = 0;
	}

	void GameObject::Update(float deltaTime)
	{
		if (m_enabled == true)
		{
			for (auto component : m_components)
			{
				if (component.second != nullptr)
				{
					component.second->OnUpdate(deltaTime);
				}
			}
		}
	}

	void GameObject::Render()
	{
		if (m_visible == true)
		{
			for (auto component : m_components)
			{
				if (component.second != nullptr)
				{
					component.second->OnRender();
				}
			}
		}
	}

	void GameObject::SetName(std::string name)
	{
		Core::Utils::isStringEmpty(name) ? m_name = S2DE_DEFAULT_GAMEOBJECT_NAME : m_name = name;
	}

	void GameObject::SetType(std::string type)
	{
		Core::Utils::isStringEmpty(type) ? m_type = S2DE_DEFAULT_GAMEOBJECT_TYPE : m_type = type;
	}

	void GameObject::SetVisible(bool visible)
	{
		m_visible = visible;
	}

	void GameObject::SetEnabled(bool enabled)
	{
		m_enabled = enabled;
	}

	void GameObject::SetPrefix(std::int32_t prefix)
	{
		m_prefix = prefix;
	}

}