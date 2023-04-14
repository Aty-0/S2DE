#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/UUID.h"
#include "Render/Buffers.h"
#include "Render/CB.h"

namespace S2DE::Render
{
	class LightGlobals
	{
	public:
		static ConstantBuffer<CB::CB_Light>*  LightConstantBuffer;


		// Creates light constant buffer etc
		static void Initialize();
		// Adding light to CB Light array and our vector
		static std::int32_t AddLight(Render::CB::PS_Light_Structure light, boost::uuids::uuid uuid);
		// Remove light from CB Light array and our vector 
		static void RemoveLight(boost::uuids::uuid uuid);
		// Updating structure for existing light
		static void SetNewLightStructure(Render::CB::PS_Light_Structure light, boost::uuids::uuid uuid);
		// Writes current lights structures to CB Light array 
		static void UpdateLights();

		[[nodiscard]] static std::int32_t GetLightCount() 
		{
			return static_cast<std::int32_t>(m_parsedLights.size());
		}

	private:
		static std::vector<std::pair<boost::uuids::uuid, 
			Render::CB::PS_Light_Structure>> m_parsedLights;

	};
}