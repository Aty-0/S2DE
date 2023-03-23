#include "Light.h"
#include "GameObjects/Base/GameObject.h"
#include "GameObjects/Components/Transform.h"

namespace S2DE::GameObjects::Components::Light
{
	Light::Light()
	{
		m_lightStructure = Render::CB::PS_Light_Structure();
	}

	Light::~Light()
	{
	}

	void Light::InitLight()
	{
		Logger::Warning("[Light] InitLight is not override %s %s", GetName().c_str(), GetOwner()->GetName().c_str());
	}

	void Light::UpdateCB()
	{
		Logger::Warning("[Light] UpdateCB is not override %s %s", GetName().c_str(), GetOwner()->GetName().c_str());
	}

	void Light::CreateIcon()
	{
		if (Core::Engine::isEditor())
		{
			m_iconSprite = GetOwner()->CreateComponent<Sprite>();
			m_iconSprite->SetBillboard(true);
			m_iconSprite->LoadTexture("engine_light_icon");
			m_iconSprite->GetOwner()->GetTransform()->SetScale(DirectX::SimpleMath::Vector3(0.6f, 0.6f, 0.6f));
			m_alpha = GetOwner()->CreateComponent<Components::AlphaComponent>();
			m_alpha->SetAlpha(true);
		}
	}

	void Light::OnDestroy()
	{
		Render::LightGlobals::RemoveLight(GetUUID());
	}

	void Light::OnCreate()
	{
		// Light first initialization
		Render::LightGlobals::AddLight(m_lightStructure, GetUUID());
		UpdateCB();
		CreateIcon();
		InitLight();
	}

	inline Math::Color<float>	Light::GetColor() const
	{
		return m_color;
	}

	inline float Light::GetStrength()  const
	{
		return m_strength;
	}

	inline float Light::GetPad() const
	{
		return m_pad;
	}

	inline float Light::GetRange() const
	{
		return m_range;
	}

	inline DirectX::SimpleMath::Vector3 Light::GetAttenuation() const
	{
		return m_attenuation;
	}

	void Light::SetPad(float pad)
	{
		m_pad = pad;
	}

	void Light::SetRange(float range)
	{
		m_range = range;
	}

	void Light::SetAttenuation(DirectX::SimpleMath::Vector3 attenuation)
	{
		m_attenuation = attenuation;
	}

	void Light::SetColor(Math::Color<float> color)
	{
		m_color = color; 

		if (Core::Engine::isEditor())
		{
			// TODO: Multiply to strength
			m_iconSprite->SetColor(color);
		}

		onColorChanged.RunAllCallbacks();
	}

	void Light::SetStrength(float strength)
	{ 
		m_strength = strength; 
		onStrengthChanged.RunAllCallbacks();
	}

}