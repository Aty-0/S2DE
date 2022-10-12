#include "AmbientLight.h"

namespace S2DE::GameObjects::Light
{
	AmbientLight::AmbientLight() 
	{

	}

	AmbientLight::~AmbientLight()
	{

	}

	void AmbientLight::UpdateCB()
	{
		// Get light constant buffer
		Render::LightConstBuff buffer = Render::LightGlobals::GlobalLightConstBuffer;

		// Upload new data
		buffer->Lock();
		buffer->GetData()->ambient_light.strength = m_strength;
		buffer->GetData()->ambient_light.color = DirectX::SimpleMath::Vector4(m_color.r, m_color.g, m_color.b, 1);
		buffer->Unlock();
		buffer->Bind(1);
		buffer->Unbind();
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
