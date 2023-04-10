#pragma once
#include "GameObjects/Components/Component.h"

#include "Math/Color.h"

#include "Render/Buffers.h"
#include "Render/CB.h"
#include "Render/LightGlobals.h"

#include "GameObjects/Components/Sprite.h"
#include "GameObjects/Components/AlphaComponent.h"

namespace S2DE::GameObjects::Components::Light
{
	enum class S2DE_API LightTypes
	{
		DefaultLight = 0,
		DirectionalLight = 1,
		PointLight = 2,
		SpotLight = 3,
		AmbientLight = 4, 
	};

	class S2DE_API Light : public Component
	{
	public:
		Core::Utils::CallbackWrapper<Core::Utils::DefaultCallback> onColorChanged;
		Core::Utils::CallbackWrapper<Core::Utils::DefaultCallback> onStrengthChanged;

		Light();
		~Light();

		void										  SetPad(float pad);
		void										  SetRange(float range);
		void										  SetAttenuation(DirectX::SimpleMath::Vector3 attenuation);
		void										  SetColor(Math::Color<float> color);
		void										  SetStrength(float strength);
		void										  OnCreate() override;
		void										  OnDestroy() override;
		void										  OnRender() override { }

		[[nodiscard]] inline Math::Color<float>	GetColor() const;
		[[nodiscard]] inline float GetStrength()  const;
		[[nodiscard]] inline float GetPad() const;
		[[nodiscard]] inline float GetRange() const;
		[[nodiscard]] inline DirectX::SimpleMath::Vector3 GetAttenuation() const;
		

	protected:
		virtual void								  CreateIcon();
		virtual void								  InitLight();
		virtual void								  UpdateCB();
	
	
		Math::Color<float>							  m_color;
		float										  m_strength;
		float										  m_pad;
		float										  m_range;
		DirectX::SimpleMath::Vector3				  m_attenuation;
		Render::CB::PS_Light_Structure				  m_lightStructure;	
	};
}