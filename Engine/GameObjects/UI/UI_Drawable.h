#pragma once
#include "GameObjects/Drawable.h"

namespace S2DE::GameObjects::UI
{
	class UI_Drawable : public Drawable
	{
	public:
		UI_Drawable() = default;
		virtual ~UI_Drawable() { }

	
	//NOTE:
	//I understand it's weird solution maybe I'm wrong and I need to remove it
	//And I try to explain why we need it
	//Because we need toggle Z Buffer when we render 2D Elements
	//It's it's kinda bit annoying

	private:
		virtual void OnRender() override;

	protected:
		virtual void OnRenderIn2D() = 0;

	};
}