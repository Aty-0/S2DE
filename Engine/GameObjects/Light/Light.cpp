#include "Light.h"

namespace S2DE::GameObjects::Light
{
	std::int32_t Light::LightCount;

	// FIX ME: We need to move it to light globals or something like that
	Render::ConstantBuffer<Render::CB::CB_Light>*  Light::m_lightCB;

	Light::Light()
	{
		// Create global light constant buffer 
		if (m_lightCB == nullptr)
		{
			m_lightCB = new Render::ConstantBuffer<Render::CB::CB_Light>();
			S2DE_ASSERT(m_lightCB->Create());

			m_lightCB->GetData()->ambient_light = Render::CB::PS_AmbientLight_Structure();
			std::fill_n(m_lightCB->GetData()->lights, MAX_LIGHTS, Render::CB::PS_Light_Structure());
		}

		m_lightStructure = Render::CB::PS_Light_Structure();
		CreateIcon();

		m_index = Light::LightCount;
		Light::LightCount++;
	}

	Light::~Light()
	{

	}

	void Light::CreateIcon() 
	{		
		m_iconSprite = new Sprite(); 
		m_iconSprite->SetBillboard(true); 
		m_iconSprite->LoadTexture("engine_light_icon");
		m_iconSprite->SetScale(DirectX::SimpleMath::Vector3(0.6f, 0.6f, 0.6f));
		// FIX ME: ....
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
}