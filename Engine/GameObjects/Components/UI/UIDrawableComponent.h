#pragma once
#include "GameObjects/Components/DrawableComponent.h"

namespace S2DE::GameObjects::Components::UI
{
	// NOTE: Needed for parse UI objects properly 
	class S2DE_API UIDrawableComponent : public DrawableComponent
	{
	public:
		UIDrawableComponent() = default;
		virtual ~UIDrawableComponent() { }
	};
}