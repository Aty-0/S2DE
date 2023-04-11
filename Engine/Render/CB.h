#pragma once
#include "Base/Main/Common.h"

namespace S2DE::Render::CB
{	
	static constexpr auto MAX_LIGHT_COUNT = 64;

	struct S2DE_ALIGN(16) PS_AmbientLight_Structure
	{
		DirectX::SimpleMath::Vector4 color = { 0, 0, 0, 0 };
		float			  strength = 0.0f;
	};

	struct S2DE_ALIGN(16) PS_Light_Structure
	{
		DirectX::SimpleMath::Vector4   position = { 0, 0, 0, 0 };
		DirectX::SimpleMath::Vector4   direction = { 0, 0, 0, 0 };
		DirectX::SimpleMath::Vector4   color = { 0, 0, 0, 0 };
		DirectX::SimpleMath::Vector3  attenuation = { 0, 0, 0 };
		
		float   spotAngle = 0.0f;
		float	range = 0.0f;
		float   pad = 0.0f;
		float	strength = 0.0f;
		float	spot = 0.0f;

		std::int32_t light_type = 0;
		std::int32_t enabled = 0;
	};

	struct S2DE_API S2DE_ALIGN(16) CB_Light
	{
		PS_Light_Structure	lights[MAX_LIGHT_COUNT] = { };
		PS_AmbientLight_Structure ambient_light = { };
	};

	struct S2DE_API S2DE_ALIGN(16) CB_Main
	{
		float						 deltatime = 0.0f;
		float						 time = 0.0f;
		DirectX::SimpleMath::Vector2 resoultion = { 0, 0 };
		DirectX::SimpleMath::Matrix  world = DirectX::SimpleMath::Matrix::Identity;
		DirectX::SimpleMath::Matrix  view = DirectX::SimpleMath::Matrix::Identity;
		DirectX::SimpleMath::Matrix  projection = DirectX::SimpleMath::Matrix::Identity;
		DirectX::SimpleMath::Vector3 cameraPosition = { 0, 0, 0 };
		DirectX::SimpleMath::Vector3 cameraRotation = { 0, 0, 0 };
	};

	struct S2DE_API S2DE_ALIGN(16) CB_Sprite
	{
		DirectX::SimpleMath::Vector2	tileFrame = { 0, 0 };
		DirectX::SimpleMath::Vector2	tileSize = { 0, 0 };
		DirectX::SimpleMath::Vector2	textureRes = { 0, 0 };
		bool							billboard = false;
	};
}
