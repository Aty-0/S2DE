#pragma once
#include "GameObjects/Light/Light.h"

//TODO: Compute normal function
//TODO: Get color from texture 

namespace S2DE::GameObjects::Light
{
	class S2DE_API AmbientLight : public Light
	{
	public:
		AmbientLight();
		virtual ~AmbientLight();

	protected:
		virtual void		 UpdateCB() override;
		virtual void		 OnChangeStrength()  override;
		virtual void		 OnChangeColor()	 override;

	};
}
