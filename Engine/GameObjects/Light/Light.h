#pragma once
#include "GameObjects/Base/Drawable.h"
#include "Render/LightGlobals.h"
#include "Math/Color.h"
#include "GameObjects/Sprite.h"

namespace S2DE::GameObjects::Light
{
	class S2DE_API Light : public Drawable // Drawable cuz we need to render light icons and other stuff in editor
	{
	public:
		Light();
		virtual ~Light();

		void										  SetPad(float pad);
		void										  SetRange(float range);
		void										  SetAttenuation(DirectX::SimpleMath::Vector3 attenuation);
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

		// TODO: Flexible callbacks
		virtual void								  OnChangePad() { }
		virtual void								  OnChangeRange() { }
		virtual void								  OnChangeAttenuation() { }
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
		std::int32_t								  m_index;
		Render::CB::PS_Light_Structure				  m_lightStructure;
	};
}