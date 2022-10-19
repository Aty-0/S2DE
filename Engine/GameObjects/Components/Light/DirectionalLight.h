#pragma once
#include "GameObjects/Components/Light/Light.h"

namespace S2DE::GameObjects::Components::Light
{
	class S2DE_API DirectionalLight : public Light
	{
	public:
		DirectionalLight();
		virtual ~DirectionalLight();
		
	protected:
		virtual void		 UpdateCB() override;
	};
}


