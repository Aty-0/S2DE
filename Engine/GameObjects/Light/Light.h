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
		Light() = default;
		virtual ~Light() { }

								 
		void										  SetColor(Math::Color<float> color);
		void										  SetStrength(float strength);
		inline Math::Color<float>					  GetColor() const { return m_color; }
		inline float								  GetStrength()  const { return m_strength; }
													  
	protected:										  
		virtual void								  OnUpdate(float DeltaTime)  override { }
		virtual void								  OnRender() override;
													  
		virtual void								  UpdateCB() { }
		virtual void								  OnChangeStrength() { }
		virtual void								  OnChangeColor()	{ }
		virtual void								  CreateIcon();

		Math::Color<float>							  m_color;
		float										  m_strength;
		Render::ConstantBuffer<Render::CB::CB_Light>* m_lightCB;
		Sprite*										  m_iconSprite;

	};
}