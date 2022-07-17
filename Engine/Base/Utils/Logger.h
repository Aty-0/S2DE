#pragma once
#include "Base/Main/Common.h"

#include <ctime>
#include <fstream>

#define S2DE_FATAL_ERROR(message) S2DE::Core::Utils::Logger::Fatal("%s\nTechnical information:\nLine:%d\nFile:%s\nin Function: %s" ,message , __LINE__, __FILE__ , __func__); 
#define S2DE_NO_IMPL() S2DE::Core::Utils::Logger::Warning("No function implementation %s", __func__);
#define S2DE_ASSERT(a) if(!(a)) { S2DE::Core::Utils::Logger::Fatal("Assertion failed %s\n File:%s\n Line:%d", #a,  __FILE__, __LINE__); }
#define S2DE_CHECK(f, r) if (!CheckHR(f, true)) { S2DE_FATAL_ERROR(r); return false; }  
#define S2DE_CHECK_SAFE(f, r) if (!CheckHR(f, true)) { S2DE::Core::Utils::Logger::Error("%s %s %d %s", #f,  __FILE__, __LINE__, r); return false; }  

namespace S2DE::Core::Utils
{
	class S2DE_API Logger
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
		static void				 Print(const char* type, const char* text);
		static std::string		 GetCorrentTime();
								 
		static std::uint32_t	 m_linecount;
		static time_t            m_time;
		static tm*				 m_localtime;
		static std::ofstream	 m_log_file;
		static std::string		 m_log_file_name;
	};

	static inline std::string GetHRCodeDetails(HRESULT hr)
	{
		_com_error err(hr);
		return std::string(err.ErrorMessage());
	}

	static inline bool CheckHR(HRESULT hr, bool print_code = false)
	{
		if (FAILED(hr))
		{
			if(print_code == true)
				Logger::Error("Error Code: 0x%08X Details: %s ", hr, GetHRCodeDetails(hr).c_str());

			return false;
		}
	
		return true;
	}
}
