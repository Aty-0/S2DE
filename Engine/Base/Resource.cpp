#include "Resource.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp> 
#include "Base/Engine.h"
#include "Base/Utils/Logger.h"
#include "Base/ResourceManager.h"

namespace S2DE::Core::Resources
{
	Resource::Resource(std::string type, std::string ex) :
		m_extensions({ ".a", ".b" }),
		m_name("DefaultResourceName"),
		m_type(type),
		m_notLoaded(true),
		m_cantDelete(false)
	{
		boost::split(m_extensions, ex, boost::is_any_of(" "), boost::token_compress_on);
	}

	Resource::Resource(std::string type, std::string ex[]) :
		m_extensions(),
		m_name("DefaultResourceName"),
		m_type(type),
		m_notLoaded(true),
		m_cantDelete(false)
	{
		//std::copy(ex, sizeof(ex), m_extensions);
	}

	Resource::Resource() : 
		m_extensions({ ".a", ".b"}),
		m_name("DefaultResourceName"),
		m_type("DefaultResourceType"),
		m_notLoaded(true),
		m_cantDelete(false)
	{

	}

	Resource::~Resource()
	{
		m_extensions.clear();
		m_name.clear();
		m_type.clear();
	}


	bool Resource::Load(std::string name) 
	{
		Utils::Logger::Warning("[Resource] Load funciton is not override");
		return true;
	}

	std::vector<std::string> Resource::FindPath(std::vector<std::string> names)
	{
		std::vector<std::string> paths = { };

		for (const auto name : names)
		{
			const static auto resourceManager = Core::Resources::ResourceManager::GetInstance();

			auto path = std::string();
			if (resourceManager->GetFilePath(name, this, path))
			{
				paths.push_back(path);
			}
			else
			{
				m_notLoaded = true;
				return { }; // Return empty vector
			}
		}

		m_notLoaded = false;

		return paths;
	}

	inline std::string Resource::GetName() const
	{
		return m_name;
	}

	inline std::string  Resource::GetType() const
	{
		return m_type;
	}

	inline std::vector<std::string>  Resource::GetExtensions() const
	{
		return m_extensions;
	}
}