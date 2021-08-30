#pragma once
#include "Base/Main/Common.h"

#include <ctime>
#include <fstream>

#define S2DE_FATAL_ERROR(message) Logger::Fatal("%s\nTechnical information:\nLine:%d\nFile:%s\nin Function: %s" ,message , __LINE__, __FILE__ , __func__); 
#define S2DE_ASSERT(a) if(!(a)) { Logger::Fatal("Assertion failed %s\n File:%s\n Line:%d", #a,  __FILE__, __LINE__); }
#define S2DE_NO_IMPL() { Logger::Warning("No function implementation %d %s %s", __LINE__, __FILE__ , __func__); }

//TODO
//S2DE_CHECK

namespace S2DE
{
	class S2DE_DX11_API Logger
	{
	public:
		Logger();
		~Logger();


		static void				 Log(const char* text, ...);
		static void				 Error(const char* text, ...);
		static void				 Warning(const char* text, ...);
		static void				 Fatal(const char* text, ...);

		static void				 CreateLogFile();

		static std::string		 GetTime(bool min_sec = false);
		static const std::string GetLogFileName() { return m_log_file_name; }

	private:
		static void				Print(const char* type, const char* text);
		static std::string		GetCorrentTime();

		static std::uint32_t	m_linecount;
		static time_t           m_time;
		static tm*				m_localtime;
		static std::ofstream	m_log_file;
		static std::string		m_log_file_name;
	};
}
