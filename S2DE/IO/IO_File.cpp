#include "IO_File.h"
#include "Base/Utils/Logger.h"

namespace S2DE
{
	void IO_File::SetFileName(std::string name)
	{
		m_name = name;
	}

	bool IO_File::Load(std::string path)
	{
		Logger::Warning("Function Load not overrided %s", m_name.c_str());
		return true;
	}

	IO_File::IO_File() : 
		m_name("DefaultName"),
		m_type("DefaultType"),
		m_ex({"DefaultExtensionA", "DefaultExtensionB"})
	{

	}

	IO_File::IO_File(std::string type, std::string ex[])
	{
		m_type = type;
		for (std::int32_t i = 0; i <= (std::int32_t)sizeof(ex); i++)
			m_ex[i] = ex[i];
	}

	IO_File::IO_File(std::string type, std::string ex)
	{
		m_type = type;
		m_ex = { { ex } };
	}
}