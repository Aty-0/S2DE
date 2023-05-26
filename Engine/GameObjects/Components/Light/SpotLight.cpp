#include "SpotLight.h"
#include "GameObjects/Base/GameObject.h"

namespace S2DE::GameObjects::Components::Light
{
	SpotLight::SpotLight()
	{

	}

	SpotLight::~SpotLight()
	{
		onColorChanged.Clear();
		onStrengthChanged.Clear();
	}

	void SpotLight::InitLight()
	{
		m_attenuation = Math::float3(0.04f, 0.1f, 0.06f);
		m_range = 100.0f;
		m_pad = 1.0f;
		m_strength = 1.0f;
		m_spot = 1.0f;
		m_spotAngle = 30.0f;

		// Add callbacks...
		const auto transform = GetOwner()->GetTransform();
		Assert(transform, "transform is nullptr");

		transform->onPositionChanged.AddCallback(std::bind(&SpotLight::UpdateCB, this));
		transform->onRotationChanged.AddCallback(std::bind(&SpotLight::UpdateCB, this));
		transform->onScaleChanged.AddCallback(std::bind(&SpotLight::UpdateCB, this));
		onColorChanged.AddCallback(std::bind(&SpotLight::UpdateCB, this));
		onStrengthChanged.AddCallback(std::bind(&SpotLight::UpdateCB, this));
	}

	void SpotLight::UpdateCB()
	{
		const static auto lightGlobals = Render::LightGlobals::GetInstance();
		const static auto renderer = Render::Renderer::GetInstance();

		lightGlobals->Begin(renderer);

		auto& str = m_lightStructure;

		str.strength = m_strength;
		str.attenuation = m_attenuation;
		str.range = m_range;
		str.spot = m_spot;
		str.spotAngle = m_spotAngle;

		str.light_type = static_cast<std::int32_t>(LightTypes::SpotLight);
		str.enabled = static_cast<std::int32_t>(GetOwner()->isEnabled());
		str.color = Math::float4(m_color.r, m_color.g, m_color.b, 1);

		const auto transform = GetOwner()->GetTransform();
		Assert(transform, "transform is nullptr");

		auto parentPosition = Math::float3::Zero;
		auto parentRotation = Math::float3::Zero;

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
			str.position = Math::float4(transform->GetPosition().x + parentPosition.x,
				transform->GetPosition().y + parentPosition.y, transform->GetPosition().z + parentPosition.z, 1);

			str.direction = Math::float4(transform->GetRotation().x + parentRotation.x,
				transform->GetRotation().y + parentRotation.y, transform->GetRotation().z + parentRotation.z, 1);
		}

		lightGlobals->SetNewLightStructure(str, GetUUID());
		lightGlobals->End(renderer);

	}
}
