#pragma once
#include "GameObjects/Base/Drawable.h"
#include "Math/Color.h"
#include "Render/Buffers.h"
#include "Render/CB.h"
#include "GameObjects/Sprite.h"

namespace S2DE::GameObjects::Light
{
	class S2DE_API Light : public Drawable // Drawable cuz we need to render light icons and other stuff in editor
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
													  
	protected:										  
		virtual void								  CreateIcon();
		virtual void								  RenderIcon();

		virtual void								  UpdateCB() { }


		virtual void								  OnRender() override;
		virtual void								  OnUpdate(float DeltaTime)  override { }
		virtual void								  OnCreate() override;
		virtual void								  OnChangeStrength() { }
		virtual void								  OnChangeColor()	{ }
		virtual void								  OnPositionChanged()  override { }
		virtual void								  OnRotationChanged()  override { }


		Math::Color<float>							  m_color;

		float										  m_strength;
		float										  m_pad;
		float										  m_range;

		DirectX::SimpleMath::Vector3				  m_attenuation;

		Sprite*										  m_iconSprite;


		static std::int32_t							  LightCount;
		std::int32_t								  m_index;

		static Render::ConstantBuffer<Render::CB::CB_Light>* m_lightCB;
		Render::CB::PS_Light_Structure				  m_lightStructure;
	};
}