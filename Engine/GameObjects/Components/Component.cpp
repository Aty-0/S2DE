#include "Component.h"
#include "Base/Utils/StringUtils.h"
#include "Base/Engine.h"
#include "Render/Renderer.h"
#include "GameObjects/Base/GameObject.h"
#include "GameObjects/Components/Transform.h"

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

	void Component::OnRenderSelected(Render::Renderer* renderer, Transform* transform)
	{		
		renderer->DebugDrawCube(transform->GetPosition(), { 0,0,0 }, Math::float3::One + transform->GetScale(),
			{ 1, 1, 1, 1 });
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

	inline std::string Component::GetName() const
	{
		return m_name;
	}

	inline std::uint32_t Component::GetPriority() const
	{
		return m_priority;
	}

	inline GameObject* Component::GetOwner() const
	{
		return m_owner;
	}

	inline bool Component::isSelected() const
	{
		return m_isSelected;
	}
}