#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/UUID.h"
#include "Render/Buffers.h" // is including renderer too
#include "Render/CB.h"

namespace S2DE::Render
{
	class LightGlobals : public Core::Utils::Singleton<LightGlobals>
	{
	public:
		LightGlobals();
		~LightGlobals();

		// Creates light constant buffer etc
		void Initialize();
		// Adding light to CB Light array and our vector
		std::int32_t AddLight(Render::CB::PS_Light_Structure light, boost::uuids::uuid uuid);
		// Remove light from CB Light array and our vector 
		void RemoveLight(boost::uuids::uuid uuid);
		// Updating structure for existing light
		void SetNewLightStructure(Render::CB::PS_Light_Structure light, boost::uuids::uuid uuid);
		// Writes current lights structures to CB Light array 
		void UpdateLights();
		// Begin track light data
		void Begin(Render::Renderer* renderer);
		// End track light data
		void End(Render::Renderer* renderer);

		// Get lights to pool 
		[[nodiscard]] inline std::int32_t GetLightCount() const
		{
			return static_cast<std::int32_t>(m_lights.size());
		}

		[[nodiscard]] inline Render::ConstantBuffer<CB::CB_Light>* GetLightConstantBuffer()
		{
			return m_lightsConstantBuffer;
		}

	private:
		std::vector<std::pair<boost::uuids::uuid, Render::CB::PS_Light_Structure>> m_lights;
		ConstantBuffer<CB::CB_Light>*  m_lightsConstantBuffer;

	};
}