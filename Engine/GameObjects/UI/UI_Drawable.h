#pragma once
#include "GameObjects/Drawable.h"

namespace S2DE::GameObjects::UI
{
	//NOTE:
	//UI_Drawable it's class with OnRenderIn2D function where we toggle Z buffer
	//For rendering objects only in 2D mode
	class UI_Drawable : public Drawable
	{
	public:
		UI_Drawable() = default;
		virtual ~UI_Drawable() { }

	private:
		virtual void OnRender() override;

	protected:
		virtual void OnRenderIn2D() = 0;

	};
}