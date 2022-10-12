#include "DirectionalLight.h"

namespace S2DE::GameObjects::Light
{
	DirectionalLight::DirectionalLight()
	{
		m_attenuation = DirectX::SimpleMath::Vector3(0.0f, 0.1f, 0.0f);
		m_range = 25.0f;
		m_pad = 1.0f;
	}

	DirectionalLight::~DirectionalLight()
	{

	}

	void DirectionalLight::UpdateCB()
	{
		// Update light structure
		m_lightStructure.attenuation = m_attenuation;
		m_lightStructure.pad = m_pad;
		m_lightStructure.light_type = static_cast<std::int32_t>(Render::LightTypes::Directional);
		m_lightStructure.enabled = static_cast<std::int32_t>(isEnabled());
		m_lightStructure.position = DirectX::SimpleMath::Vector4(GetPosition().x, GetPosition().y, GetPosition().z, 1);
		m_lightStructure.direction = DirectX::SimpleMath::Vector4(GetRotation().x, GetRotation().y, GetRotation().z, 1);
		m_lightStructure.color = DirectX::SimpleMath::Vector4(m_color.r, m_color.g, m_color.b, 1);

		// Get light constant buffer
		Render::LightConstBuff buffer = Render::LightGlobals::GlobalLightConstBuffer;

		// Upload new data
		buffer->Lock();
		buffer->GetData()->lights[m_index] = m_lightStructure;
		buffer->Unlock();
		buffer->Bind(1);
		buffer->Unbind();
	}

	void DirectionalLight::OnPositionChanged()
	{
		if (m_iconSprite)
		{
			m_iconSprite->SetPosition(GetPosition());
		}


		UpdateCB();
	}

	void DirectionalLight::OnRotationChanged()
	{
		UpdateCB();
	}

	void DirectionalLight::OnChangeStrength()
	{
		UpdateCB();
	}

	void DirectionalLight::OnChangeColor()
	{
		UpdateCB();
	}
}
