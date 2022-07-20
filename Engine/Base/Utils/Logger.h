#pragma once
#include "Base/Main/Common.h"

#include <ctime>
#include <fstream>

#define S2DE_FATAL_ERROR(message) S2DE::Core::Utils::Logger::Fatal("%s\nTechnical information:\nLine:%d\nFile:%s\nin Function: %s" ,message , __LINE__, __FILE__ , __func__); 
#define S2DE_NO_IMPL() S2DE::Core::Utils::Logger::Warning("No function implementation %s", __func__);
#define S2DE_ASSERT(a) if(!(a)) { S2DE::Core::Utils::Logger::Fatal("Assertion failed %s\n File:%s\n Line:%d", #a,  __FILE__, __LINE__); }
#define S2DE_CHECK(f, r) if (!S2DE::Core::Utils::Logger::CheckHR(f, true)) { S2DE_FATAL_ERROR(r); return false; }  
#define S2DE_CHECK_SAFE(f, r) if (!S2DE::Core::Utils::Logger::CheckHR(f, true)) { S2DE::Core::Utils::Logger::Error("%s %s %d %s", #f,  __FILE__, __LINE__, r); return false; }  

namespace S2DE::Core::Utils
{
	class S2DE_API Logger
	{
	public:
		Logger();
		~Logger();

		static void					Log(const char* text, ...);
		static void					Error(const char* text, ...);
		static void					Warning(const char* text, ...);
		static void					Fatal(const char* text, ...);
		static void					CreateLogFile();


		static std::string			GetTime(bool printMinAndSec = false);
		static const std::string		GetLogFileName() { return m_logFileName; }
		static std::string			GetHRCodeDetails(HRESULT hr);
		static bool					CheckHR(HRESULT hr, bool printCode = false);

	private:
		static std::uint32_t		m_lineCount;
		static time_t				m_time;
		static tm*					m_localTime;
		static std::ofstream		m_logFile;
		static std::string			m_logFileName;
		static void					Print(const char* type, const char* text);
		static std::string			GetCorrentTime();
	public:
		static std::vector<std::function<void()>> onPrintCallbacks;
	};
}
