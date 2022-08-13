#pragma once
#include "Base/Main/Common.h"

namespace S2DE::Render::CB
{
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
