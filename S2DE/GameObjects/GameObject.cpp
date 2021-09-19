#include "GameObject.h"


namespace S2DE
{
	GameObject::GameObject() :
		m_name(std::string()),
		m_type(std::string()),
		m_prefix(0),
		m_visible(true),
		m_enabled(true),
		m_id(new GameObjectIDGenerator())
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

	void GameObject::RenderDebugGUI()
	{
		if (m_visible == true)
			OnDebugRenderGUI();
	}

	void GameObject::Render()
	{
		if (m_visible == true)
			OnRender();
	}

	void GameObject::SetName(std::string name)
	{
		m_name = name;
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

	void GameObject::SetType(std::string type)
	{
		m_type = type;
	}

	void GameObject::Init(std::string name, std::string type, std::int32_t prefix, std::string id)
	{
		isStringEmpty(name) ? m_name = "GameObject" : m_name = name;
		isStringEmpty(type) ? m_type = "Default" : m_type = type;
		m_prefix = prefix;

		//If id not empty we set that ID
		if (!isStringEmpty(id))
			m_id->SetID(id);

		OnCreate();
	}
}