#include "DirectionalLight.h"
#include "GameObjects/Base/GameObject.h"

namespace S2DE::GameObjects::Components::Light
{
	DirectionalLight::DirectionalLight()
	{
		m_index = Light::LightCount;
		Light::LightCount++;
		m_attenuation = DirectX::SimpleMath::Vector3(0.0f, 0.1f, 0.0f);
		m_range = 25.0f;
		m_pad = 1.0f;
	}

	DirectionalLight::~DirectionalLight()
	{
		Light::LightCount--;
	}

	void DirectionalLight::UpdateCB()
	{
		if (m_lightCB)
		{
			m_lightCB->Lock();

			m_lightStructure.attenuation = m_attenuation;
			m_lightStructure.pad = m_pad;
			m_lightStructure.light_type = static_cast<std::int32_t>(LightTypes::DIRECTIONAL_LIGHT);

			auto transform = GetOwner()->GetTransform();
			m_lightStructure.position = DirectX::SimpleMath::Vector4(transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z, 1);
			m_lightStructure.direction = DirectX::SimpleMath::Vector4(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, 1);

			m_lightStructure.color = DirectX::SimpleMath::Vector4(m_color.r, m_color.g, m_color.b, 1);
			m_lightStructure.color = DirectX::SimpleMath::Vector4(m_color.r, m_color.g, m_color.b, 1);

			m_lightCB->GetData()->lights[m_index] = m_lightStructure;

			m_lightCB->Unlock();
			m_lightCB->Bind(1);
			m_lightCB->Unbind();
		}
	}
}
