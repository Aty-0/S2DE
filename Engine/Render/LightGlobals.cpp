#include "LightGlobals.h"

namespace S2DE::Render
{
	ConstantBuffer<CB::CB_Light>* LightGlobals::LightConstantBuffer;
	std::vector<std::pair<boost::uuids::uuid,
		Render::CB::PS_Light_Structure>> LightGlobals::m_parsedLights;

	void LightGlobals::Initialize()
	{
		if (LightConstantBuffer == nullptr)
		{
			// Create constant buffer 
			LightConstantBuffer = new Render::ConstantBuffer<Render::CB::CB_Light>();
			S2DE_ASSERT(LightConstantBuffer->Create());

			LightConstantBuffer->GetData()->ambient_light = Render::CB::PS_AmbientLight_Structure();

			// Create light array 
			// LightConstantBuffer->GetData()->lights = new Render::CB::PS_Light_Structure[MAX_LIGHT_COUNT]();
			// Fill array not null structures
			std::fill_n(LightConstantBuffer->GetData()->lights, CB::MAX_LIGHT_COUNT, Render::CB::PS_Light_Structure());
		}
		else
		{
			Logger::Warning("LightGlobals is already initialized!");
		}
	}

	void LightGlobals::SetNewLightStructure(Render::CB::PS_Light_Structure light, boost::uuids::uuid uuid)
	{
		const auto it = std::find_if(m_parsedLights.begin(), m_parsedLights.end(), [&uuid](std::pair<boost::uuids::uuid,
			Render::CB::PS_Light_Structure> const& el) { return el.first == uuid; });

		if (it != m_parsedLights.end())
		{
			const auto index = static_cast<std::int32_t>(it - m_parsedLights.begin());
			m_parsedLights.at(index).second = light; // Rewrite in vector 
			//UpdateLights();
			LightConstantBuffer->GetData()->lights[index] = light; // Set new structure to array 
		}	
	}

	void LightGlobals::UpdateLights()
	{
		for (const auto& element : m_parsedLights)
		{
			const auto uuid = element.first;
			const auto it = std::find_if(m_parsedLights.begin(), m_parsedLights.end(), [&uuid](std::pair<boost::uuids::uuid,
				Render::CB::PS_Light_Structure> const& el) { return el.first == uuid; });
			
			if (it != m_parsedLights.end())
			{
				Render::LightGlobals::LightConstantBuffer->Lock();
				const auto index = static_cast<std::int32_t>(it - m_parsedLights.begin());
				LightConstantBuffer->GetData()->lights[index] = element.second;
				Render::LightGlobals::LightConstantBuffer->Unlock();
				Render::LightGlobals::LightConstantBuffer->Bind(1);
				Render::LightGlobals::LightConstantBuffer->Unbind();
			}
		}
	}

	std::int32_t LightGlobals::AddLight(Render::CB::PS_Light_Structure light, boost::uuids::uuid uuid)
	{
		if (m_parsedLights.size() < CB::MAX_LIGHT_COUNT)
		{
			Render::LightGlobals::LightConstantBuffer->Lock();
			m_parsedLights.push_back(std::make_pair(uuid, light));

			// Parse elements in vector, then set it by vector index into array 
	
			const auto it = std::find_if(m_parsedLights.begin(), m_parsedLights.end(), [&uuid](std::pair<boost::uuids::uuid,
				Render::CB::PS_Light_Structure> const& el) { return el.first == uuid; });

			if (it != m_parsedLights.end())
			{
				Logger::Log("LightGlobals Add light %s", Core::Utils::UUID::ConvertUUIDToString(uuid).c_str());

				auto index = static_cast<std::int32_t>(it - m_parsedLights.begin());
				LightConstantBuffer->GetData()->lights[index] = m_parsedLights.at(index).second;
				return index;
			}
			Render::LightGlobals::LightConstantBuffer->Unlock();
			Render::LightGlobals::LightConstantBuffer->Bind(1);
			Render::LightGlobals::LightConstantBuffer->Unbind();
		}
		else
		{
			Logger::Error("LightGlobals can't add new light, array size is overflow");
			return -1;
		}

		return 0;
	}

	void LightGlobals::RemoveLight(boost::uuids::uuid uuid)
	{
		const auto it = std::find_if(m_parsedLights.begin(), m_parsedLights.end(), [&uuid](std::pair<boost::uuids::uuid,
			Render::CB::PS_Light_Structure> const& el) { return el.first == uuid; });

		if (it != m_parsedLights.end())
		{
			Render::LightGlobals::LightConstantBuffer->Lock();
			Logger::Log("LightGlobals Remove light %s", Core::Utils::UUID::ConvertUUIDToString(uuid).c_str());

			m_parsedLights.erase(it);
			// Reset array 
			std::fill_n(LightConstantBuffer->GetData()->lights, CB::MAX_LIGHT_COUNT, Render::CB::PS_Light_Structure());
			// Write new parsedLights
			UpdateLights();

			Render::LightGlobals::LightConstantBuffer->Unlock();
			Render::LightGlobals::LightConstantBuffer->Bind(1);
			Render::LightGlobals::LightConstantBuffer->Unbind();
		}

	}
}