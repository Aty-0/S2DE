#include "VisualConsole.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"

#define S2DE_CONSOLE_DEFAULT_MODE VisualConsoleSizeMode::Fullscreen

namespace S2DE::Core::Debug
{
	std::vector<std::string> VisualConsole::ConsoleBuffer;

	VisualConsole::VisualConsole() : 
		m_scroll_to_bottom(false), 
		m_free(Engine::isEditor()),
		m_sizemode(S2DE_CONSOLE_DEFAULT_MODE)
	{ 

	}

	VisualConsole::~VisualConsole()
	{

	}

	void VisualConsole::Clear()
	{
		ConsoleBuffer.clear();
	}

	void VisualConsole::Scroll()
	{
		m_scroll_to_bottom = true;
	}

	void VisualConsole::UpdateSizeMode()
	{
		switch (m_sizemode)
		{
		case VisualConsoleSizeMode::Default:
			ImGui::SetWindowSize("Console", ImVec2(float(Engine::GetGameWindow()->GetWidth() - 10), 400.0f));
			break;
		case VisualConsoleSizeMode::Fullscreen:
			ImGui::SetWindowSize("Console", ImVec2(float(Engine::GetGameWindow()->GetWidth() - 10), float(Engine::GetGameWindow()->GetHeight() - 32.0f)));
			break;
		case VisualConsoleSizeMode::Half:
			ImGui::SetWindowSize("Console", ImVec2(float(Engine::GetGameWindow()->GetWidth() / 2), float(Engine::GetGameWindow()->GetHeight() - 35.0f)));
			break;
		}
	}

	void VisualConsole::Render()
	{
		if (!m_draw)
			return;

		ImGui::Begin("Console", &m_draw,
			m_free == false ? 
			  ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
			  ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
			  ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar 

			: 0);

		//Reset position if it's not free mode
		if (m_free == false)
		{
			UpdateSizeMode();
			ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
		}

		if (ImGui::Button("Open log file"))
			ShellExecute(0, 0, Logger::GetLogFileName().c_str(), 0, 0, SW_SHOW);


		ImGui::SameLine();

		if (ImGui::Button("Clear"))
			ConsoleBuffer.clear();


		//Free check box
		//////////////////////////////////////////////////////
		if (!Engine::isEditor())
		{
			ImGui::SameLine();
			ImGui::Checkbox("Free mode", &m_free);
			ImGui::SameLine();
		}

		//Size slider
		//////////////////////////////////////////////////////


		if (!Engine::isEditor())
		{
			static std::int32_t mode = static_cast<std::int32_t>(S2DE_CONSOLE_DEFAULT_MODE);
			m_sizemode = static_cast<VisualConsoleSizeMode>(mode);
			ImGui::PushItemWidth(100);
			if (ImGui::SliderInt("Mode", &mode, 0, 2, "", 0))
				UpdateSizeMode();
		}


		//Separate 
		ImGui::Separator();

		//Console text	
		//////////////////////////////////////////////////////
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, 
			ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysVerticalScrollbar |
			ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysHorizontalScrollbar);
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

		for (std::int32_t i = 0; i < (std::int32_t)ConsoleBuffer.size(); i++)
		{
			std::string item = ConsoleBuffer[i].c_str();

			ImVec4 col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); 

			std::int32_t max_lines = 175 + Engine::GetGameWindow()->GetWidth() / 50;

			if (item.length() >= max_lines)
			{
				for (std::int32_t i = 1; i <= std::int32_t(item.length() / max_lines); i++)
					item.insert(max_lines * i, "\n");
			}

			if (strstr(item.c_str(), "[Error]")) 
				col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
			else if (strstr(item.c_str(), "[Fatal]"))
				col = ImColor(1.0f, 0.0f, 0.0f, 1.0f);
			else if (strstr(item.c_str(), "[Warning]"))
				col = ImColor(0.8f, 0.8f, 0.0f, 1.0f);
	

			ImGui::PushStyleColor(ImGuiCol_Text, col);
			ImGui::TextUnformatted(item.c_str());
			ImGui::PopStyleColor();
		}

		if (m_scroll_to_bottom)
			ImGui::SetScrollHereY(1.0f);

		m_scroll_to_bottom = false;
		ImGui::PopStyleVar();
		ImGui::EndChild();

		ImGui::End();
	}
}