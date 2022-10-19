#pragma once
#include "GameObjects/Components/Light/Light.h"

// TODO: Get color from texture 

namespace S2DE::GameObjects::Components::Light
{
	class S2DE_API AmbientLight : public Light
	{
	public:
		AmbientLight();
		virtual ~AmbientLight();

	protected:
		virtual void		 UpdateCB() override;
	};
}
