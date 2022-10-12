#include "Light.h"

namespace S2DE::GameObjects::Light
{
	Light::Light()
	{
		m_lightStructure = Render::CB::PS_Light_Structure();
		CreateIcon();
		
		m_index = Render::LightGlobals::LightCount;
		Render::LightGlobals::LightCount++;
	}

	Light::~Light()
	{
		Render::LightGlobals::LightCount--;
		Core::Delete(m_iconSprite);
	}

	void Light::CreateIcon() 
	{		
		m_iconSprite = new Sprite(); 
		m_iconSprite->SetBillboard(true); 
		m_iconSprite->LoadTexture("engine_light_icon");
		m_iconSprite->SetScale(DirectX::SimpleMath::Vector3(0.6f, 0.6f, 0.6f));
		Alpha = true;
	}

	void Light::OnCreate()
	{
		// Light first initialization
		UpdateCB();
	}

	void Light::RenderIcon()
	{
		if (Core::Engine::isEditor())
			 m_iconSprite->Render(); 
	}

	void Light::OnRender()
	{
		RenderIcon();
	}

	void Light::SetColor(Math::Color<float> color)
	{ 
		m_color = color; 
		
		if (m_iconSprite) // TODO: Multiply to strength
			m_iconSprite->SetColor(color);

		OnChangeColor(); 
	}

	void Light::SetStrength(float strength)
	{ 
		m_strength = strength; 
		OnChangeStrength();
	}

	void Light::SetPad(float pad)
	{
		m_pad = pad;
		OnChangePad();
	}

	void Light::SetRange(float range)
	{
		m_range = range;
		OnChangeRange();
	}

	void Light::SetAttenuation(DirectX::SimpleMath::Vector3 attenuation)
	{
		m_attenuation = attenuation;
		OnChangeAttenuation();
	}
}