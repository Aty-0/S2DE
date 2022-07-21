#include "GameObject.h"

namespace S2DE::GameObjects
{
	GameObject::GameObject() :
		m_name(std::string()),
		m_type(std::string()),
		m_prefix(0),
		m_visible(true),
		m_enabled(true),
		m_id(new GameObjectIDGenerator()),
		m_isSelected(false)
	{								  

	}

	GameObject::~GameObject()
	{
		OnDestroy();

		m_type.clear();
		m_name.clear();
		m_id = nullptr;
		m_prefix = 0;
	}

	void GameObject::Update(float DeltaTime)
	{
		if (m_enabled == true)
			OnUpdate(DeltaTime);
	}

	void GameObject::UpdateInput()
	{
		if (m_enabled == true)
			OnUpdateInput();
	}

	void GameObject::RenderImGUI()
	{
		if (m_visible == true)
			OnRenderImGUI();
	}

	void GameObject::Render()
	{
		if (m_visible == true)
			OnRender();
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

	void GameObject::Init(std::string name, std::string type, std::int32_t prefix, std::string id)
	{
		//Set name, type and prefix 
		SetName(name);
		SetType(type);
		SetPrefix(prefix);

		//TODO: Need check special id on invalid
		if(id == "REGENERATE")
			m_id->RegenerateUUID();
		else if (!Core::Utils::isStringEmpty(id))
			m_id->SetUUID(id);

		OnCreate();
	}
}