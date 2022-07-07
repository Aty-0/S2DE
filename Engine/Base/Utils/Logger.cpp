#include "Logger.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "Base/Main/BuildDate.h"
#include "Base/DebugTools/VisualConsole.h"

#include "Graphics/Renderer.h"

#pragma warning(disable:4996)

#include <cstdarg> 


#define VA_LIST_OUTPUT(buffer) 	va_list args; \
		va_start(args, text); \
		std::vsnprintf(buffer, sizeof(buffer), text, args); \
		va_end(args);	\

namespace S2DE::Core::Utils
{
	std::uint32_t		Logger::m_linecount;
	time_t				Logger::m_time;
	tm*					Logger::m_localtime;
	std::ofstream		Logger::m_log_file;
	std::string			Logger::m_log_file_name;

	Logger::Logger()
	{
	
	}


	Logger::~Logger()
	{
		m_linecount = NULL;
		m_time = NULL;
	
		Delete(m_localtime);
		m_log_file.close();
	}

	std::string Logger::GetTime(bool min_sec)
	{
		m_time = time(0);
		m_localtime = localtime(&m_time);

		std::string min_sec_Str = min_sec == true ? std::to_string(m_localtime->tm_sec) + std::to_string(m_localtime->tm_min) : std::string();

		return   min_sec_Str + std::to_string(m_localtime->tm_mday) + std::to_string(1 + m_localtime->tm_mon) + std::to_string(1900 + m_localtime->tm_year);
	}

	std::string Logger::GetCorrentTime()
	{
		m_time = time(0);
		m_localtime = localtime(&m_time);

		return std::to_string(1900 + m_localtime->tm_year) + "." + std::to_string(1 + m_localtime->tm_mon) + "." +
			std::to_string(m_localtime->tm_mday) + " " + std::to_string(m_localtime->tm_hour) +
			":" + std::to_string(m_localtime->tm_min) + ":" + std::to_string(m_localtime->tm_sec);
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
		//TODO
		//Add log date in file name
		m_log_file_name = "S2DE.log";

		m_log_file = std::ofstream(m_log_file_name, std::ios_base::out);

		//Main text 

		m_log_file << 
			"------------------------------------------------\n" <<
			"- S2DE Log file\n" << 
			"------------------------------------------------\n" <<
			"- Build: " << S2DE_BUILD_DATE << "\n" <<
			"------------------------------------------------" <<
			"\n\n";

		m_log_file.close();
	}

	void Logger::Print(const char* type, const char* text)
	{
		std::string line;

		line.append("[" + std::to_string(m_linecount) + "] ");

		//Add to line corrent time
		line.append(GetCorrentTime());
		//Add type and text from buffer
		line.append(" [" + std::string(type) + "]  " + std::string(text) + "\n");

		//print to vs console
		OutputDebugString(line.c_str());

		//Add text to log file
		m_log_file.open(m_log_file_name, std::ios::app | std::ios_base::out);
		m_log_file << line;
		m_log_file.close();

		Debug::VisualConsole::ConsoleBuffer.push_back(line);

		//Add line 
		m_linecount++;

		if (Core::Engine::GetRenderer() != nullptr)
			if (Core::Engine::GetRenderer()->GetImGui_Window("EngineConsole") != nullptr)
				reinterpret_cast<Debug::VisualConsole*>(Core::Engine::GetRenderer()->GetImGui_Window("EngineConsole"))->Scroll();

		line.clear();
	}
}