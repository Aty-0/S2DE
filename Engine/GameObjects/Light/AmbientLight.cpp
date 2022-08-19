#include "AmbientLight.h"

namespace S2DE::GameObjects::Light
{
	AmbientLight::AmbientLight() 
	{
		m_lightCB = new Render::ConstantBuffer<Render::CB::CB_Light>();
		S2DE_ASSERT(m_lightCB->Create());
		CreateIcon();
	}

	AmbientLight::~AmbientLight()
	{
		Core::Delete(m_lightCB);
	}

	void AmbientLight::UpdateCB()
	{
		m_lightCB->Lock();
		m_lightCB->GetData()->ambient_light.strength = m_strength;
		m_lightCB->GetData()->ambient_light.color = DirectX::SimpleMath::Vector3(m_color.r, m_color.g, m_color.b);
		m_lightCB->Unlock();

		m_lightCB->Bind(1);
		m_lightCB->Unbind();
	}

	void AmbientLight::OnChangeStrength()
	{
		UpdateCB();
	}

	void AmbientLight::OnChangeColor()
	{
		UpdateCB();
	}
}
