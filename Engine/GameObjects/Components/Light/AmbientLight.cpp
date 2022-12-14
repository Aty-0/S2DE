#include "AmbientLight.h"
#include "GameObjects/Base/GameObject.h"

namespace S2DE::GameObjects::Components::Light
{
	AmbientLight::AmbientLight() 
	{

	}

	AmbientLight::~AmbientLight()
	{
		onColorChanged.Clear();
		onStrengthChanged.Clear();
	}

	void AmbientLight::InitLight()
	{
		m_strength = 1.0f;
		onColorChanged.AddCallback(std::bind(&AmbientLight::UpdateCB, this));
		onStrengthChanged.AddCallback(std::bind(&AmbientLight::UpdateCB, this));
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
