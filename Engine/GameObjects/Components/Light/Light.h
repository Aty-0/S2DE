#pragma once
#include "GameObjects/Components/Component.h"

#include "Math/Color.h"

#include "Render/Buffers.h"
#include "Render/CB.h"

#include "GameObjects/Components/Sprite.h"
#include "GameObjects/Components/AlphaComponent.h"

namespace S2DE::GameObjects::Components::Light
{
	class S2DE_API Light : public Component 
	{
	public:
		enum LightTypes
		{
			DEFAULT_LIGHT = 0, 
			DIRECTIONAL_LIGHT = 1,
			POINT_LIGHT = 2,
			SPOT_LIGHT  = 3,
		};

		Light();
		virtual ~Light();

								 
		void										  SetColor(Math::Color<float> color);
		void										  SetStrength(float strength);
		inline Math::Color<float>					  GetColor() const { return m_color; }
		inline float								  GetStrength()  const { return m_strength; }

	public:											  
		std::vector<std::function<void()>>			  onColorChanged;
		std::vector<std::function<void()>>			  onStrengthChanged;
		
	protected:										  
		virtual void								  CreateIcon();
		virtual void								  RenderIcon();

		virtual void								  UpdateCB() { }


		virtual void								  OnRender() override;
		virtual void								  OnUpdate(float DeltaTime)  override { }
		virtual void								  OnCreate() override;

		virtual void								  PositionChanged();
		virtual void								  ColorChanged();
		virtual void								  StrengthChanged();
		

		Math::Color<float>							  m_color;

		float										  m_strength;
		float										  m_pad;
		float										  m_range;

		DirectX::SimpleMath::Vector3				  m_attenuation;

		Components::AlphaComponent*					  m_alpha;
		Sprite*										  m_iconSprite;


		Render::CB::PS_Light_Structure				  m_lightStructure;
		std::int32_t								  m_index;

		// TODO: Move to globals
		static Render::ConstantBuffer<Render::CB::CB_Light>* m_lightCB;
		static std::int32_t							  LightCount;

		
	};
}