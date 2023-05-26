#include "LightGlobals.h"

namespace S2DE::Render
{
	LightGlobals::LightGlobals()
	{

	}

	LightGlobals::~LightGlobals()
	{
		m_lights.clear();
		m_lights.shrink_to_fit();

		Core::Delete(m_lightsConstantBuffer);
	}


	void LightGlobals::Initialize()
	{
		Core::Utils::Logger::Log("LightGlobals initialize...");
		// Create constant buffer 
		m_lightsConstantBuffer = new Render::ConstantBuffer<Render::CB::CB_Light>();
		Assert(m_lightsConstantBuffer->Create(), "Failed to create light buffer");

		m_lightsConstantBuffer->GetData()->ambient_light = Render::CB::PS_AmbientLight_Structure();

		// Create light array 
		// LightConstantBuffer->GetData()->lights = new Render::CB::PS_Light_Structure[MAX_LIGHT_COUNT]();
		// Fill array not null structures
		std::fill_n(m_lightsConstantBuffer->GetData()->lights, CB::MAX_LIGHT_COUNT, Render::CB::PS_Light_Structure());
	}

	void LightGlobals::SetNewLightStructure(Render::CB::PS_Light_Structure light, boost::uuids::uuid uuid)
	{
		const auto it = std::find_if(m_lights.begin(), m_lights.end(), [&uuid](std::pair<boost::uuids::uuid,
			Render::CB::PS_Light_Structure> const& el) { return el.first == uuid; });

		if (it != m_lights.end())
		{
			const auto index = static_cast<std::int32_t>(it - m_lights.begin());
			m_lights.at(index).second = light; // Rewrite in vector 
			//UpdateLights();
			m_lightsConstantBuffer->GetData()->lights[index] = light; // Set new structure to array 
		}	
	}

	void LightGlobals::UpdateLights()
	{
		for (const auto& element : m_lights)
		{
			const auto uuid = element.first;
			const auto it = std::find_if(m_lights.begin(), m_lights.end(), [&uuid](std::pair<boost::uuids::uuid,
				Render::CB::PS_Light_Structure> const& el) { return el.first == uuid; });
			
			if (it != m_lights.end())
			{
				static const auto renderer = Render::Renderer::GetInstance();

				Begin(renderer);
				const auto index = static_cast<std::int32_t>(it - m_lights.begin());
				m_lightsConstantBuffer->GetData()->lights[index] = element.second;

				End(renderer);
			}
		}
	}

	std::int32_t LightGlobals::AddLight(Render::CB::PS_Light_Structure light, boost::uuids::uuid uuid)
	{
		if (m_lights.size() < CB::MAX_LIGHT_COUNT)
		{
			static const auto renderer = Render::Renderer::GetInstance();
			Begin(renderer);

			m_lights.push_back(std::make_pair(uuid, light));

			// Parse elements in vector, then set it by vector index into array 
	
			const auto it = std::find_if(m_lights.begin(), m_lights.end(), [&uuid](std::pair<boost::uuids::uuid,
				Render::CB::PS_Light_Structure> const& el) { return el.first == uuid; });

			if (it != m_lights.end())
			{
				Core::Utils::Logger::Log("LightGlobals Add light %s", Core::Utils::UUID::ConvertUUIDToString(uuid).c_str());

				auto index = static_cast<std::int32_t>(it - m_lights.begin());
				m_lightsConstantBuffer->GetData()->lights[index] = m_lights.at(index).second;
				return index;
			}

			End(renderer);
		}
		else
		{
			Core::Utils::Logger::Error("LightGlobals can't add new light, array size is overflow");
			return -1;
		}

		return 0;
	}

	void LightGlobals::RemoveLight(boost::uuids::uuid uuid)
	{
		const auto it = std::find_if(m_lights.begin(), m_lights.end(), [&uuid](std::pair<boost::uuids::uuid,
			Render::CB::PS_Light_Structure> const& el) { return el.first == uuid; });

		if (it != m_lights.end())
		{
			static const auto renderer = Render::Renderer::GetInstance();
			m_lightsConstantBuffer->Lock(renderer);
			Core::Utils::Logger::Log("LightGlobals Remove light %s", Core::Utils::UUID::ConvertUUIDToString(uuid).c_str());

			m_lights.erase(it);
			// Reset array 
			std::fill_n(m_lightsConstantBuffer->GetData()->lights, CB::MAX_LIGHT_COUNT, Render::CB::PS_Light_Structure());
			// Write new parsedLights
			UpdateLights();


		}

	}
	void LightGlobals::Begin(Render::Renderer* renderer)
	{
		m_lightsConstantBuffer->Lock(renderer);
	}

	void LightGlobals::End(Render::Renderer* renderer)
	{
		m_lightsConstantBuffer->Unlock(renderer);
		m_lightsConstantBuffer->Bind(renderer, 1);
		m_lightsConstantBuffer->Unbind(renderer);
	}
}