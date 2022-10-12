#pragma once
#include "Render/Buffers.h"
#include "Render/CB.h"

namespace S2DE::Render
{
	typedef ConstantBuffer<CB::CB_Light>* LightConstBuff;

	enum LightTypes
	{
		Default = 0,
		Directional = 1,
		Point = 2,
		Spot = 3,
	};

	class LightGlobals
	{
	public:
		static LightConstBuff	GlobalLightConstBuffer;
		static std::int32_t	    LightCount;

		static void CreateLightConstantBuffer()
		{
			// Create global light constant buffer 
			GlobalLightConstBuffer = new Render::ConstantBuffer<Render::CB::CB_Light>();
			S2DE_ASSERT(GlobalLightConstBuffer->Create());
			GlobalLightConstBuffer->GetData()->ambient_light = Render::CB::PS_AmbientLight_Structure();
			std::fill_n(GlobalLightConstBuffer->GetData()->lights, MAX_LIGHTS, Render::CB::PS_Light_Structure());
		}

		static void DeleteLightConstantBuffer()
		{
			Core::Delete(GlobalLightConstBuffer);
		}
	};
}