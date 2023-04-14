#pragma once
#include "GameObjects/Components/Light/Light.h"

namespace S2DE::GameObjects::Components::Light
{
	class S2DE_API SpotLight : public Light
	{
	public:
		SpotLight();
		~SpotLight();

	protected:
		void		 UpdateCB() override;
		void		 InitLight() override;

	private:
		float m_spot;
		float m_spotAngle;
	};
}
