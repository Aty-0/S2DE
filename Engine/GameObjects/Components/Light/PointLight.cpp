#include "PointLight.h"
#include "GameObjects/Base/GameObject.h"
#include "Render/Renderer.h"

namespace S2DE::GameObjects::Components::Light
{
	PointLight::PointLight()
	{

	}

	PointLight::~PointLight()
	{
		onColorChanged.Clear();
		onStrengthChanged.Clear();
	}

	void PointLight::OnRenderSelected(Render::Renderer* renderer, Transform* transform)
	{
		renderer->DebugDrawSphere({ transform->GetPosition().x - (m_range / 32), transform->GetPosition().y, transform->GetPosition().z - (m_range / 32) }, m_range,
			{ m_color.r, m_color.g, m_color.b, m_color.a });
	}

	void PointLight::InitLight()
	{
		//m_attenuation = DirectX::SimpleMath::Vector3(0.04f, 0.1f, 0.06f);
		m_attenuation = DirectX::SimpleMath::Vector3(3.0f, 0.0f, 1.0f);
		m_range = 100.0f;
		m_pad = 1.0f;
		m_strength = 1.0f;

		// Add callbacks...
		const auto transform = GetOwner()->GetTransform();
		Assert(transform, "transform is nullptr");

		transform->onPositionChanged.AddCallback(std::bind(&PointLight::UpdateCB, this));
		transform->onRotationChanged.AddCallback(std::bind(&PointLight::UpdateCB, this));
		transform->onScaleChanged.AddCallback(std::bind(&PointLight::UpdateCB, this));
		onColorChanged.AddCallback(std::bind(&PointLight::UpdateCB, this));
		onStrengthChanged.AddCallback(std::bind(&PointLight::UpdateCB, this));
	}

	void PointLight::UpdateCB()
	{
		Render::LightGlobals::LightConstantBuffer->Lock();

		auto& str = m_lightStructure;

		str.strength = m_strength;
		str.attenuation = m_attenuation;
		str.range = m_range;
		str.pad = m_pad;
		str.light_type = static_cast<std::int32_t>(LightTypes::PointLight);
		str.enabled = static_cast<std::int32_t>(GetOwner()->isEnabled());
		str.color = DirectX::SimpleMath::Vector4(m_color.r, m_color.g, m_color.b, 1);

		const auto transform = GetOwner()->GetTransform();
		Assert(transform, "transform is nullptr");

		auto parentPosition = DirectX::SimpleMath::Vector3::Zero;
		auto parentRotation = DirectX::SimpleMath::Vector3::Zero;

		// TODO: Get global position, rotation, scale 
		//		 This is local p, r, s
		if (transform->GetParent() != nullptr)
		{
			auto transformParent = transform->GetParent()->GetTransform();

			if (transformParent != nullptr)
			{
				parentPosition = transformParent->GetPosition();
				parentRotation = transformParent->GetRotation();
			}
		}


		if (transform != nullptr)
		{
			str.position = DirectX::SimpleMath::Vector4(transform->GetPosition().x + parentPosition.x,
				transform->GetPosition().y + parentPosition.y, transform->GetPosition().z + parentPosition.z, 1);

			str.direction = DirectX::SimpleMath::Vector4(transform->GetRotation().x + parentRotation.x,
				transform->GetRotation().y + parentRotation.y, transform->GetRotation().z + parentRotation.z, 1);
		}


		Render::LightGlobals::SetNewLightStructure(str, GetUUID());


		Render::LightGlobals::LightConstantBuffer->Unlock();
		Render::LightGlobals::LightConstantBuffer->Bind(1);
		Render::LightGlobals::LightConstantBuffer->Unbind();
		
	}
}
