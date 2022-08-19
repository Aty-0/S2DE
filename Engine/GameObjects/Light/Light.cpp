#include "Light.h"

namespace S2DE::GameObjects::Light
{
	void Light::CreateIcon() 
	{		
		m_iconSprite = new Sprite(); 
		m_iconSprite->SetBillboard(true); 
		m_iconSprite->LoadTexture("engine_light_icon");
		m_iconSprite->SetScale(DirectX::SimpleMath::Vector3(0.6f, 0.6f, 0.6f));
		//FIX ME:
		Alpha = true;
	}

	void Light::OnRender()
	{
		if (Core::Engine::isEditor())
			 m_iconSprite->Render(); 
	}

	void Light::SetColor(Math::Color<float> color)
	{ 
		m_color = color; 
		OnChangeColor(); 
	}

	void Light::SetStrength(float strength)
	{ 
		m_strength = strength; 
		OnChangeStrength();
	}
}