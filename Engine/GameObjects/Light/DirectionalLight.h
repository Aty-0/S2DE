#pragma once
#include "GameObjects/Light/Light.h"

namespace S2DE::GameObjects::Light
{
	class S2DE_API DirectionalLight : public Light
	{
	public:
		DirectionalLight();
		virtual ~DirectionalLight();
		
	protected:
		virtual void		 UpdateCB() override;
		virtual void		 OnPositionChanged()  override;
		virtual void		 OnRotationChanged()  override;
		virtual void		 OnChangeStrength()  override;
		virtual void		 OnChangeColor()	 override;

	};
}


