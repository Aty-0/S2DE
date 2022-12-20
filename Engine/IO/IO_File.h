#pragma once
#include "Base/Main/Common.h"

namespace S2DE::IO
{
	class S2DE_API IO_File
	{
	public:
		IO_File();
		IO_File(std::string type, std::string ex[]);
		IO_File(std::string type, std::string ex);


		[[nodiscard]] virtual std::string	GetName() const { return m_name; }
		[[nodiscard]] virtual std::string	GetType() const { return m_type; }
		[[nodiscard]] virtual std::vector<std::string>  GetExtension() const { return m_ex; }
		virtual void   SetFileName(std::string name);
		virtual bool   Load(std::string name);

	protected:
		std::string m_name;
		std::string m_type;
		std::vector<std::string> m_ex;
	};
}