#include "Light.h"
#include "GameObjects/Base/GameObject.h"
#include "GameObjects/Components/Transform.h"

namespace S2DE::GameObjects::Components::Light
{
	std::int32_t Light::LightCount;
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

		m_index = Light::LightCount;
		Light::LightCount++;
	}

	Light::~Light()
	{

	}

	void Light::ColorChanged()
	{
		onColorChanged.RunAllCallbacks();
	}

	void Light::StrengthChanged()
	{
		onStrengthChanged.RunAllCallbacks();
	}

	void Light::InitLight()
	{
		
	}

	void Light::PositionChanged()
	{
		UpdateCB();
	}

	void Light::UpdateCB()
	{
		Logger::Warning("[Light] UpdateCB is not override %s %s", GetName().c_str(), GetOwner()->GetName().c_str());
	}

	void Light::CreateIcon() 
	{		
		m_iconSprite = GetOwner()->CreateComponent<Sprite>();
		m_iconSprite->SetBillboard(true); 
		m_iconSprite->LoadTexture("engine_light_icon");
		m_iconSprite->GetOwner()->GetTransform()->SetScale(DirectX::SimpleMath::Vector3(0.6f, 0.6f, 0.6f));
		m_alpha = GetOwner()->CreateComponent<Components::AlphaComponent>();
		m_alpha->SetAlpha(true);
	}

	void Light::OnCreate()
	{
		// Light first initialization
		UpdateCB();
		CreateIcon();
		InitLight();
	}

	void Light::RenderIcon()
	{
		if (Core::Engine::isEditor())
			 m_iconSprite->OnRender(); 
	}

	void Light::OnRender()
	{
		RenderIcon();
	}

	void Light::SetColor(Math::Color<float> color)
	{
		Logger::Log("Light::SetColor(Math::Color<float> color)");
		m_color = color; 
		
		if (m_iconSprite) // TODO: Multiply to strength
			m_iconSprite->SetColor(color);

		ColorChanged();
	}

	void Light::SetStrength(float strength)
	{ 
		Logger::Log("Light::SetStrength(float strength)");
		m_strength = strength; 
		StrengthChanged();
	}

}