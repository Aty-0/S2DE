#include "Logger.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "Base/Main/BuildDate.h"
#include "Base/DebugTools/VisualConsole.h"

#include <filesystem>

#pragma warning(disable:4996)

#include <cstdarg> 


#define VA_LIST_OUTPUT(buffer) 	va_list args; \
		va_start(args, text); \
		std::vsnprintf(buffer, sizeof(buffer), text, args); \
		va_end(args);	\

namespace S2DE::Core::Utils
{
	std::uint32_t		Logger::m_lineCount;
	time_t				Logger::m_time;
	tm*					Logger::m_localTime;
	std::ofstream		Logger::m_logFile;
	std::string			Logger::m_logFileName;
	std::vector<std::function<void()>> Logger::onPrintCallbacks;

	Logger::Logger()
	{
	
	}

	Logger::~Logger()
	{
		Delete(m_localTime);
		m_time = NULL;	
		m_logFile.close();
		m_lineCount = 0;
	}

	const char* Logger::ParseArgsToString(const char* text, ...)
	{
		char buffer[2048];
		VA_LIST_OUTPUT(buffer);
		return buffer;
	}

	std::string Logger::GetTime(bool printMinAndSec)
	{
		m_time = time(0);
		m_localTime = localtime(&m_time);

		const char* textparsed = printMinAndSec == true ? 
			ParseArgsToString("%i.%i.%i.%i.%i", (1900 + m_localTime->tm_year), (1 + m_localTime->tm_mon), m_localTime->tm_mday,m_localTime->tm_sec, m_localTime->tm_min) :
			ParseArgsToString("%i.%i.%i", (1900 + m_localTime->tm_year), (1 + m_localTime->tm_mon), m_localTime->tm_mday);

		return textparsed;
	}

	std::string Logger::GetCorrentTime()
	{
		m_time = time(0);
		m_localTime = localtime(&m_time);
		const char* textparsed = ParseArgsToString("%i.%i.%i:%i:%i", (1900 + m_localTime->tm_year), (1 + m_localTime->tm_mon), m_localTime->tm_mday, m_localTime->tm_sec, m_localTime->tm_min);
		return textparsed;
	}

	void Logger::Fatal(const char* text, ...)
	{
		char buffer[1024];
		VA_LIST_OUTPUT(buffer);
		Print(DirectX::SimpleMath::Color(1.0f, 0.0f, 0.0f, 1.0f), "Error", std::string(buffer).c_str());
		memset(buffer, 0, sizeof(buffer));
	}

	void Logger::LogColored(DirectX::SimpleMath::Color color, const char* text, ...)
	{
		char buffer[1024];
		VA_LIST_OUTPUT(buffer);
		Print(color, "Log", std::string(buffer).c_str());
		memset(buffer, 0, sizeof(buffer));
	}

	void Logger::Log(const char* text, ...)
	{
		char buffer[1024];
		VA_LIST_OUTPUT(buffer);
		Print(DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f), "Log", std::string(buffer).c_str());
		memset(buffer, 0, sizeof(buffer));
	}

	void Logger::Warning(const char* text, ...)
	{
		char buffer[1024];
		VA_LIST_OUTPUT(buffer);
		Print(DirectX::SimpleMath::Color(0.8f, 0.8f, 0.0f, 1.0f), "Warning", std::string(buffer).c_str());
		memset(buffer, 0, sizeof(buffer));
	}

	void Logger::Error(const char* text, ...)
	{
		char buffer[1024];
		VA_LIST_OUTPUT(buffer);
		Print(DirectX::SimpleMath::Color(1.0f, 0.4f, 0.4f, 1.0f), "Error", std::string(buffer).c_str());
		memset(buffer, 0, sizeof(buffer));
	}

	void Logger::CreateLogFile()
	{
		// Create folder "Logs" if it's not exist
		if (!std::filesystem::is_directory("Logs") || !std::filesystem::exists("Logs"))
		{
			if (!std::filesystem::create_directory("Logs"))
			{
				throw std::runtime_error("Can't create directory");
			}
		}

		// Set log name
		m_logFileName.append("S2DE-Log-");
		m_logFileName.append(GetTime(true)); 
		m_logFileName.append(".log");

		// Create log file 
		m_logFile = std::ofstream("Logs/" + m_logFileName, std::ios_base::out);
		m_logFile << 
			"------------------------------------------------\n" <<
			"- S2DE Log file\n" << 
			"------------------------------------------------\n" <<
			"- Build: " << S2DE_BUILD_DATE << "\n" <<
			"------------------------------------------------" <<
			"\n\n";
	}

	void Logger::Print(DirectX::SimpleMath::Color color, const char* type, const char* text)
	{
		std::string line = std::string();
		// Print line count
		line.append("[" + std::to_string(m_lineCount) + "] ");

		// Add to line corrent time
		line.append(GetCorrentTime());

		// Add type and text from buffer
		line.append(" [" + std::string(type) + "]  " + std::string(text) + "\n");

		// If we are have debuging output
		OutputDebugString(line.c_str());

		// Add text to log file
		m_logFile << line;

		Debug::ConsoleBufferObject object = Debug::ConsoleBufferObject();
		object.text = line;
		object.color = color;

		Debug::VisualConsole::ConsoleBuffer.push_back(object);

		// Add line 
		m_lineCount++;

		// Run callbacks
		for (const auto& func : onPrintCallbacks)
		{
			if (func != nullptr)
			{
				func();
			}
		}

		line.clear();
	}

	std::string Logger::GetHRCodeDetails(HRESULT hr)
	{
		_com_error err(hr);
		return std::string(err.ErrorMessage());
	}

	bool Logger::CheckHR(HRESULT hr, bool printCode)
	{
		if (FAILED(hr))
		{
			if (printCode == true)
			{
				Logger::Error("Error Code: 0x%08X Details: %s ", hr, GetHRCodeDetails(hr).c_str());
			}

			return false;
		}
		return true;
	}
}