#include "VisualConsole.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"

#define S2DE_CONSOLE_DEFAULT_MODE VisualConsoleSizeMode::Fullscreen

namespace S2DE::Core::Debug
{
	VisualConsole::VisualConsole() : 
		m_scroll_to_bottom(false), 
		m_draw(false),
		m_free(false),
		m_sizemode(S2DE_CONSOLE_DEFAULT_MODE)
	{ 

	}


	VisualConsole::~VisualConsole()
	{

	}

	void VisualConsole::Clear()
	{
		m_buffer.clear();
	}

	void VisualConsole::TougleDraw()
	{
		m_draw =! m_draw;
	}

	void VisualConsole::Add(std::string text)
	{
		m_buffer.push_back(text);
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
			ImGui::SetWindowSize("Console", ImVec2(float(Engine::GetGameWindow()->GetWidth() - 10), float(Engine::GetGameWindow()->GetHeight() - 35.0f)));
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
			  ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
			  ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar 

			: ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings);

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
			m_buffer.clear();

		ImGui::SameLine();

		//Free check box
		//////////////////////////////////////////////////////
		ImGui::Checkbox("Free mode", &m_free);


		ImGui::SameLine();

		//Size slider
		//////////////////////////////////////////////////////
		static std::int32_t mode = static_cast<std::int32_t>(S2DE_CONSOLE_DEFAULT_MODE);
		m_sizemode = static_cast<VisualConsoleSizeMode>(mode);



		ImGui::PushItemWidth(100);
		if (ImGui::SliderInt("Mode", &mode, 0, 2, "", 0))
			UpdateSizeMode();
		


		//Separate 
		ImGui::Separator();

		//Console text	
		//////////////////////////////////////////////////////
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, 
			ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysVerticalScrollbar |
			ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysHorizontalScrollbar);
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

		for (std::int32_t i = 0; i < (std::int32_t)m_buffer.size(); i++)
		{
			const char* item = m_buffer[i].c_str();

			ImVec4 col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); 

			if (strstr(item, "[Error]")) 
				col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
			else if (strstr(item, "[Fatal]"))
				col = ImColor(1.0f, 0.0f, 0.0f, 1.0f);
			else if (strstr(item, "[Warning]"))
				col = ImColor(0.8f, 0.8f, 0.0f, 1.0f);
	

			ImGui::PushStyleColor(ImGuiCol_Text, col);
			ImGui::TextUnformatted(item);
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