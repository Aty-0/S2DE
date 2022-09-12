#pragma once
#include "Base/Main/Common.h"

#define MAX_LIGHTS 8

namespace S2DE::Render::CB
{	
	struct S2DE_ALIGN(16) PS_AmbientLight_Structure
	{
		DirectX::SimpleMath::Vector4 color;
		float			  strength;
	};

	struct S2DE_ALIGN(16) PS_Light_Structure
	{
		DirectX::SimpleMath::Vector4   position;
		DirectX::SimpleMath::Vector4   direction;
		DirectX::SimpleMath::Vector4   color;
		DirectX::SimpleMath::Vector3  attenuation;

		float   spotAngle;
		float	range;
		float   pad;
		float	strength;

		int     light_type;
		int		enabled;

	};

	struct S2DE_API S2DE_ALIGN(16) CB_Light
	{
		PS_Light_Structure		  lights[MAX_LIGHTS];
		PS_AmbientLight_Structure ambient_light;
	};

	struct S2DE_API S2DE_ALIGN(16) CB_Main
	{
		float						 deltatime;
		float						 time;
		DirectX::SimpleMath::Vector2 resoultion;
		DirectX::SimpleMath::Matrix  world;
		DirectX::SimpleMath::Matrix  view;
		DirectX::SimpleMath::Matrix  projection;
		DirectX::SimpleMath::Vector3 cameraPosition;
	};

	struct S2DE_API S2DE_ALIGN(16) CB_Sprite
	{
		DirectX::SimpleMath::Vector2	tileFrame;
		DirectX::SimpleMath::Vector2	tileSize;
		DirectX::SimpleMath::Vector2	textureRes;
		bool							billboard;
	};
}
