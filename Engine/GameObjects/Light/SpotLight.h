#pragma once
#include "GameObjects/Light/Light.h"

namespace S2DE::GameObjects::Light
{
	// TODO: Texture 
	class S2DE_API SpotLight : public Light
	{
	public:
		SpotLight();
		virtual ~SpotLight();
		virtual void		 SetSpot(float spot);

	protected:
		virtual void		 OnChangeSpot();
		virtual void		 UpdateCB() override;
		virtual void		 OnPositionChanged()  override;
		virtual void		 OnRotationChanged()  override;
		virtual void		 OnChangeStrength()  override;
		virtual void		 OnChangeColor()	 override;
		virtual void		 OnChangePad() override;
		virtual void		 OnChangeRange()  override;
		virtual void		 OnChangeAttenuation()  override;

	private:
		float				 m_spot;

	};
}

