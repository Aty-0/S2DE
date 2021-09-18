#include "VisualConsole.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"

#define S2DE_CONSOLE_DEFAULT_MODE static_cast<std::int32_t>(VisualConsoleSizeMode::Fullscreen)

namespace S2DE
{
	VisualConsole::VisualConsole() : 
		m_scroll_to_bottom(false), 
		m_draw(false),
		m_free(false)
	{ 

	}


	VisualConsole::~VisualConsole()
	{

	}

	void VisualConsole::Clear()
	{
		m_imgui_text_buffer.clear();
	}

	void VisualConsole::TougleDraw()
	{
		m_draw =! m_draw;
	}

	void VisualConsole::Add(std::string text)
	{
		m_imgui_text_buffer.appendf(text.c_str());
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
			ImGui::SetWindowSize("Console", ImVec2(float(Engine::GetGameWindow()->GetWidth() - 10), float(Engine::GetGameWindow()->GetHeight() - 30)));
			break;
		case VisualConsoleSizeMode::Half:
			ImGui::SetWindowSize("Console", ImVec2(float(Engine::GetGameWindow()->GetWidth() / 2), float(Engine::GetGameWindow()->GetHeight() - 30)));
			break;
		}
	}

	void VisualConsole::Render()
	{
		if (!m_draw)
			return;

		ImGui::Begin("Console", &m_draw,
			m_free == false ? 
			  ImGuiWindowFlags_::ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_::ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings 
			| ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar

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
			m_imgui_text_buffer.clear();

		ImGui::SameLine();

		//Free check box
		//////////////////////////////////////////////////////
		ImGui::Checkbox("Free mode", &m_free);


		ImGui::SameLine();

		//Size slider
		//////////////////////////////////////////////////////
		static std::int32_t m = S2DE_CONSOLE_DEFAULT_MODE;
		m_sizemode = static_cast<VisualConsoleSizeMode>(m);



		ImGui::PushItemWidth(100);
		if (ImGui::SliderInt("Mode", &m, 0, 2, "", 0))
			UpdateSizeMode();
		


		//Separate 
		ImGui::Separator();

		//Console text	
		//////////////////////////////////////////////////////
		ImGui::BeginChild("scrolling");
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
		ImGui::TextUnformatted(m_imgui_text_buffer.begin());

		if (m_scroll_to_bottom)
			ImGui::SetScrollHereY(1.0f);

		m_scroll_to_bottom = false;
		ImGui::PopStyleVar();
		ImGui::EndChild();

		ImGui::End();
	}
}