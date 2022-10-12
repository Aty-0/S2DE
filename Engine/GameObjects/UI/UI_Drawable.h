#pragma once
#include "GameObjects/Base/Drawable.h"

namespace S2DE::GameObjects::UI
{
	//NOTE: Needed for parse UI objects properly 
	class S2DE_API UI_Drawable : public Drawable
	{
	public:
		UI_Drawable() = default;
		virtual ~UI_Drawable() { }
	};
}