#include "Component.h"
#include "Base/Utils/StringUtils.h"
#include "Base/Engine.h"

namespace S2DE::GameObjects::Components
{
	Component::Component() :
		m_owner(nullptr),
		m_isSelected(false),
		m_name(std::string()),
		m_priority(0)
	{

	}

	Component::~Component()
	{
		m_owner = nullptr;
		m_isSelected = false;
		m_name = std::string();
		m_priority = 0;
	}

	void Component::SetName(std::string name)
	{
		m_name = name;
	}

	void Component::SetOwner(GameObject* go)
	{
		m_owner = go;
	}

	void Component::SetPriority(std::uint32_t priority)
	{
		m_priority = priority;
	}
}