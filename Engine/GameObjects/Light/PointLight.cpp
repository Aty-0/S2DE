#include "PointLight.h"

namespace S2DE::GameObjects::Light
{
	PointLight::PointLight()
	{
		m_attenuation = DirectX::SimpleMath::Vector3(0.04f, 0.1f, 0.06f);
		m_range = 100.0f;
		m_pad = 1.0f;
		m_strength = 1.0f;
	}

	PointLight::~PointLight()
	{
		Light::LightCount--;
	}

	void PointLight::UpdateCB()
	{
		if (m_lightCB)
		{								
			m_lightCB->Lock();	

			m_lightStructure.strength				= m_strength;
			m_lightStructure.attenuation			= m_attenuation;
			m_lightStructure.range					= m_range;
			m_lightStructure.pad				    = m_pad;
			m_lightStructure.light_type				= static_cast<std::int32_t>(LightTypes::POINT_LIGHT);
			m_lightStructure.enabled				= static_cast<std::int32_t>(isEnabled());
			m_lightStructure.position				= DirectX::SimpleMath::Vector4(GetPosition().x, GetPosition().y, GetPosition().z, 1);
			m_lightStructure.direction				= DirectX::SimpleMath::Vector4(GetRotation().x, GetRotation().y, GetRotation().z, 1);
			m_lightStructure.color					= DirectX::SimpleMath::Vector4(m_color.r, m_color.g, m_color.b, 1);

			m_lightCB->GetData()->lights[m_index] = m_lightStructure;

			m_lightCB->Unlock();
			m_lightCB->Bind(1);
			m_lightCB->Unbind();
		}
	}

	void PointLight::OnPositionChanged()  
	{
		if (m_iconSprite)
		{
			m_iconSprite->SetPosition(GetPosition());
		}


		UpdateCB();
	}

	void PointLight::OnRotationChanged()  
	{
		UpdateCB();
	}
	
	void PointLight::OnChangeStrength()
	{
		UpdateCB();
	}

	void PointLight::OnChangeColor()
	{
		UpdateCB();
	}
}
