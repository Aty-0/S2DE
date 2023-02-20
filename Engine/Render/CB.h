#pragma once
#include "Base/Main/Common.h"

namespace S2DE::Render::CB
{	
	static constexpr auto MAX_LIGHT_COUNT = 64;

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
		float	spot;

		std::int32_t light_type;
		std::int32_t enabled;
	};

	struct S2DE_API S2DE_ALIGN(16) CB_Light
	{
		PS_Light_Structure	lights[MAX_LIGHT_COUNT];
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
		DirectX::SimpleMath::Vector3 cameraRotation;
	};

	struct S2DE_API S2DE_ALIGN(16) CB_Sprite
	{
		DirectX::SimpleMath::Vector2	tileFrame;
		DirectX::SimpleMath::Vector2	tileSize;
		DirectX::SimpleMath::Vector2	textureRes;
		bool							billboard;
	};
}
