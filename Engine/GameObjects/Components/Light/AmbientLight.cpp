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

	void AmbientLight::OnRenderSelected(Render::Renderer* renderer, Transform* transform)
	{
		renderer->DebugDrawCube(transform->GetPosition(), { 0,0,0 }, transform->GetScale(),
			{ m_color.r, m_color.g, m_color.b, m_color.a });
	}

	void AmbientLight::UpdateCB()
	{	
		static const auto lightGlobals = Render::LightGlobals::GetInstance();
		static const auto renderer = Render::Renderer::GetInstance();

		lightGlobals->Begin(renderer);
		const auto buffer = lightGlobals->GetLightConstantBuffer();

		buffer->GetData()->ambient_light.strength = m_strength;
		buffer->GetData()->ambient_light.color = Math::float4(m_color.r, m_color.g, m_color.b, 1);
		
		lightGlobals->End(renderer);
	}
}
