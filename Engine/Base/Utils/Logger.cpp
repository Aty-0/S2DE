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

	std::string Logger::GetTime(bool printMinAndSec)
	{
		m_time = time(0);
		m_localTime = localtime(&m_time);
		std::string minSec = printMinAndSec == true ? "." + std::to_string(m_localTime->tm_sec) + "." + std::to_string(m_localTime->tm_min) : std::string();
		return std::to_string(1900 + m_localTime->tm_year) + "." + std::to_string(1 + m_localTime->tm_mon) + "." + std::to_string(m_localTime->tm_mday) + minSec;
	}

	std::string Logger::GetCorrentTime()
	{
		m_time = time(0);
		m_localTime = localtime(&m_time);

		return std::to_string(1900 + m_localTime->tm_year) + "." + std::to_string(1 + m_localTime->tm_mon) + "." +
			std::to_string(m_localTime->tm_mday) + " " + std::to_string(m_localTime->tm_hour) +
			":" + std::to_string(m_localTime->tm_min) + ":" + std::to_string(m_localTime->tm_sec);
	}

	void Logger::Fatal(const char* text, ...)
	{
		char buffer[1024];
		VA_LIST_OUTPUT(buffer);
		Print("Fatal", std::string(buffer).c_str());
		throw std::runtime_error(std::string(buffer));
		memset(buffer, 0, sizeof(buffer));
	}

	void Logger::Log(const char* text, ...)
	{
		char buffer[1024];
		VA_LIST_OUTPUT(buffer);
		Print("Log", std::string(buffer).c_str());
		memset(buffer, 0, sizeof(buffer));
	}

	void Logger::Warning(const char* text, ...)
	{
		char buffer[1024];
		VA_LIST_OUTPUT(buffer);
		Print("Warning", std::string(buffer).c_str());
		memset(buffer, 0, sizeof(buffer));
	}

	void Logger::Error(const char* text, ...)
	{
		char buffer[1024];
		VA_LIST_OUTPUT(buffer);
		Print("Error", std::string(buffer).c_str());
		memset(buffer, 0, sizeof(buffer));
	}

	void Logger::CreateLogFile()
	{
		// Create folder "Logs" if it's not exist
		if (!std::filesystem::is_directory("Logs") || !std::filesystem::exists("Logs"))
			std::filesystem::create_directory("Logs");
		// Set log name
		m_logFileName = "S2DE-Log-" + GetTime(true) + ".log";
		// Create log file 
		m_logFile = std::ofstream("Logs/" + m_logFileName, std::ios_base::out);
		m_logFile << 
			"------------------------------------------------\n" <<
			"- S2DE Log file\n" << 
			"------------------------------------------------\n" <<
			"- Build: " << S2DE_BUILD_DATE << "\n" <<
			"------------------------------------------------" <<
			"\n\n";
		m_logFile.close();
	}

	void Logger::Print(const char* type, const char* text)
	{
		std::string line = std::string();

		line.append("[" + std::to_string(m_lineCount) + "] ");

		//Add to line corrent time
		line.append(GetCorrentTime());
		//Add type and text from buffer
		line.append(" [" + std::string(type) + "]  " + std::string(text) + "\n");

		//print to vs console
		OutputDebugString(line.c_str());

		//Add text to log file
		m_logFile.open("Logs/" + m_logFileName, std::ios::app | std::ios_base::out);
		m_logFile << line;
		m_logFile.close();

		Debug::VisualConsole::ConsoleBuffer.push_back(line);

		//Add line 
		m_lineCount++;

		for (const auto& func : onPrintCallbacks)
			func();
	
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
				Logger::Error("Error Code: 0x%08X Details: %s ", hr, GetHRCodeDetails(hr).c_str());
			return false;
		}
		return true;
	}
}