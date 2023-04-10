#pragma once
#include "GameObjects/Components/Light/Light.h"

namespace S2DE::GameObjects::Components::Light
{
	class S2DE_API PointLight : public Light
	{
	public:
		PointLight();
		~PointLight();

	protected:
		void		 UpdateCB() override;
		void		 InitLight() override;
		void		 OnRender() override;

	};
}
