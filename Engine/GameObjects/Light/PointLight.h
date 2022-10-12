#pragma once
#include "GameObjects/Light/Light.h"

namespace S2DE::GameObjects::Light
{
	class S2DE_API PointLight : public Light
	{
	public:
		PointLight();
		virtual ~PointLight();

	protected:
		virtual void		 UpdateCB() override;
		virtual void		 OnPositionChanged()  override;
		virtual void		 OnRotationChanged()  override;
		virtual void		 OnChangeStrength()  override;
		virtual void		 OnChangeColor()	 override;
		virtual void		 OnChangePad() override;
		virtual void		 OnChangeRange()  override;
		virtual void		 OnChangeAttenuation()  override;

	};
}
