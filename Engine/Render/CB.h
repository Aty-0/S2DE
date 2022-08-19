#pragma once
#include "Base/Main/Common.h"

namespace S2DE::Render::CB
{	
	struct S2DE_API S2DE_ALIGN(16) CB_Light
	{
		// TODO: struct for point, spot lights...
		struct ambient_light_t
		{
			DirectX::SimpleMath::Vector3 color;
			float						 strength;
		};

		ambient_light_t ambient_light;
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
