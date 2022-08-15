#pragma once
#include "Base/Main/Common.h"

#define MAX_AMBIENT_LIGHTS 4 

namespace S2DE::Render::CB
{	
	struct S2DE_API S2DE_ALIGN(16) CB_Light
	{
		// TODO: struct for point, spot lights...
		struct ambient_ligth_t
		{
			DirectX::SimpleMath::Vector3 color;
			float						 strength;
		};

		ambient_ligth_t ambient_lights[MAX_AMBIENT_LIGHTS];
	};

	struct S2DE_API CB_Main
	{
		float						 deltatime;
		float						 time;
		DirectX::SimpleMath::Vector2 resoultion;
		DirectX::SimpleMath::Matrix  world;
		DirectX::SimpleMath::Matrix  view;
		DirectX::SimpleMath::Matrix  projection;
	};

	struct S2DE_API S2DE_ALIGN(16) CB_Sprite
	{
		DirectX::SimpleMath::Vector2	tileFrame;
		DirectX::SimpleMath::Vector2	tileSize;
		DirectX::SimpleMath::Vector2	textureRes;
	};
}
