#pragma once
#include "GameObjects/Drawable.h"

namespace S2DE::GameObjects::UI
{
	//NOTE: Needed for parse UI objects properly 
	class UI_Drawable : public Drawable
	{
	public:
		UI_Drawable() = default;
		virtual ~UI_Drawable() { }
	};
}