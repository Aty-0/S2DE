#include "AmbientLight.h"

namespace S2DE::GameObjects::Components::Light
{
	AmbientLight::AmbientLight() 
	{

	}

	AmbientLight::~AmbientLight()
	{

	}

	void AmbientLight::UpdateCB()
	{
		if (m_lightCB)
		{
			m_lightCB->Lock();

			m_lightCB->GetData()->ambient_light.strength = m_strength;
			m_lightCB->GetData()->ambient_light.color = DirectX::SimpleMath::Vector4(m_color.r, m_color.g, m_color.b, 1);
			m_lightCB->Unlock();

			m_lightCB->Bind(1);
			m_lightCB->Unbind();
		}
	}
}
