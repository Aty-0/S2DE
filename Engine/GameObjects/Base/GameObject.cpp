#include "GameObject.h"

#include "Scene/SceneManager.h"

#include "Render/Renderer.h"

#include "GameObjects/Components/Sprite.h"
#include "GameObjects/Components/DrawableComponent.h"
#include "GameObjects/Components/AlphaComponent.h"

#include "Editor/EditorObjectInspector.h"

namespace S2DE::GameObjects
{
	GameObject::GameObject() :
		m_name(S2DE_DEFAULT_GAMEOBJECT_NAME),
		m_type(S2DE_DEFAULT_GAMEOBJECT_TYPE),
		m_prefix(S2DE_DEFAULT_GAMEOBJECT_PREFIX),
		m_visible(true),
		m_enabled(true),
		m_isSelected(false)
	{		
		// This is the basic setup for game object 
		RegenerateUUID();
		m_transform = CreateComponent<Components::Transform>();
	}

	GameObject::GameObject(std::string name, std::string type, std::int32_t prefix, std::string uuid) :
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

		if (uuid == S2DE_UUID_REGENERATE)
		{
			RegenerateUUID();
		}
		else if (!Core::Utils::isStringEmpty(uuid))
		{
			if(!SetUUID(uuid))
			{
				Core::Utils::Logger::Error("Invalid UUID! GameObject Name: %s UUID Input: %s", m_name.c_str(), uuid.c_str());
			}
		}

		//OnCreate();

		m_transform = CreateComponent<Components::Transform>();

		if (Core::Engine::isEditor() 
			&& m_type != S2DE_ENGINE_GAMEOBJECT_TYPE)
		{
			// TODO: Can be disable, but we can't delete it
			//		And... we need to hide it in object inspector!

			// FIX ME: This is KEKW fix of 
			// some problems with our basic render priority
			
			GameObject* go = Scene::CreateGameObjectNoInit<GameObject>();
			go->SetName("__Icon__"); // Name is actualy not important for this
			go->GetTransform()->SetParent(this);

			m_objectIconSprite = go->CreateComponent<Components::Sprite>();
			m_objectIconSprite->SetTwoSidedMode(true);
			m_objectIconSprite->SetBillboard(true);
			m_objectIconSprite->LoadTexture("engine_game_object");

			const auto alpha = go->CreateComponent<Components::AlphaComponent>();
			alpha->SetAlpha(true);
		}
	}

	GameObject::~GameObject()
	{
		//OnDestroy();
		for (const auto component : m_components)
			component.second->OnDestroy();

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
			for (const auto component : m_components)
			{
				if (component.second != nullptr)
				{
					component.second->OnUpdate(deltaTime);
				}
			}
		}
	}

	void GameObject::Render(Render::Renderer* renderer)
	{
		if (m_visible == true)
		{
			for (const auto component : m_components)
			{
				if (component.second != nullptr)
				{
					component.second->OnRender(renderer);
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

		const auto child = GetTransform()->GetChild();
		if (child != nullptr)
		{
			child->SetVisible(visible);
		}
	}

	void GameObject::SetEnabled(bool enabled)
	{
		m_enabled = enabled;

		const auto child = GetTransform()->GetChild();
		if (child != nullptr)
		{
			child->SetEnabled(enabled);
		}
	}

	void GameObject::SetPrefix(std::int32_t prefix)
	{
		m_prefix = prefix;
	}

	inline Components::Transform* GameObject::GetTransform() const
	{
		return m_transform;
	}

	inline ComponentsList GameObject::GetComponentsList() const
	{
		return m_components;
	}

	inline std::string GameObject::GetName()   const
	{
		return m_name;
	}

	inline std::int32_t GameObject::GetPrefix() const
	{
		return m_prefix;
	}

	inline std::string GameObject::GetType()   const
	{
		return m_type;
	}

	inline bool GameObject::isVisible() const
	{
		return m_visible;
	}

	inline bool GameObject::isEnabled() const
	{
		return m_enabled;
	}

	inline bool GameObject::isSelected() const
	{
		return m_isSelected;
	}

	inline Components::Sprite* GameObject::GetIcon()   const
	{
		if (!Core::Engine::isEditor())
		{
			Logger::Warning("!!! You trying to get sprite icon in game mode, in game mode icon is not created!");
		}

		return m_objectIconSprite;
	}

}