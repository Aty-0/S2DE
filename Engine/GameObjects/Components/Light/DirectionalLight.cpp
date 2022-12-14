#include "DirectionalLight.h"
#include "GameObjects/Base/GameObject.h"

namespace S2DE::GameObjects::Components::Light
{
	DirectionalLight::DirectionalLight()
	{

	}

	DirectionalLight::~DirectionalLight()
	{
		//Light::LightCount--;
	}

	void DirectionalLight::InitLight()
	{
		m_index = Light::LightCount;
		Light::LightCount++;

		m_attenuation = DirectX::SimpleMath::Vector3(0.0f, 0.1f, 0.0f);
		m_range = 25.0f;
		m_pad = 1.0f;

		// Add callbacks...
		const auto transform = GetOwner()->GetTransform();
		S2DE_ASSERT(transform != nullptr);

		transform->onPositionChanged.AddCallback(std::bind(&DirectionalLight::UpdateCB, this));
		transform->onRotationChanged.AddCallback(std::bind(&DirectionalLight::UpdateCB, this));
		transform->onScaleChanged.AddCallback(std::bind(&DirectionalLight::UpdateCB, this));
		onColorChanged.AddCallback(std::bind(&DirectionalLight::UpdateCB, this));
		onStrengthChanged.AddCallback(std::bind(&DirectionalLight::UpdateCB, this));
	}

	void DirectionalLight::UpdateCB()
	{
		if (m_lightCB)
		{
			m_lightCB->Lock();

			m_lightStructure.attenuation = m_attenuation;
			m_lightStructure.pad = m_pad;
			m_lightStructure.light_type = static_cast<std::int32_t>(LightTypes::DIRECTIONAL_LIGHT);

			const auto transform = GetOwner()->GetTransform();
			if (transform != nullptr)
			{
				m_lightStructure.position = DirectX::SimpleMath::Vector4(transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z, 1);
				m_lightStructure.direction = DirectX::SimpleMath::Vector4(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, 1);
			}

			m_lightStructure.color = DirectX::SimpleMath::Vector4(m_color.r, m_color.g, m_color.b, 1);

			m_lightCB->GetData()->lights[m_index] = m_lightStructure;

			m_lightCB->Unlock();
			m_lightCB->Bind(1);
			m_lightCB->Unbind();
		}
	}
}
