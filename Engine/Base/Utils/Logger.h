#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/LoggerDebugTools.h"
#include <ctime>
#include <fstream>

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

		static void					LogColored(DirectX::SimpleMath::Color color, const char* text, ...);

		static void					CreateLogFile();

		[[nodiscard]] static std::string			GetTime(bool printMinAndSec = false);
		[[nodiscard]] static std::string			GetLogFileName() { return m_logFileName; }
		[[nodiscard]] static std::string			GetHRCodeDetails(HRESULT hr);
		[[nodiscard]] static bool					CheckHR(HRESULT hr, bool printCode = false);

		static const char*			ParseArgsToString(const char* text, ...);
	private:
		static std::uint32_t		m_lineCount;
		static time_t				m_time;
		static tm*					m_localTime;
		static std::ofstream		m_logFile;
		static std::string			m_logFileName;
		
		static void									Print(DirectX::SimpleMath::Color color, const char* type, const char* text);
		[[nodiscard]] static std::string			GetCorrentTime();

	public:
		static std::vector<std::function<void()>> onPrintCallbacks;
	};
}
