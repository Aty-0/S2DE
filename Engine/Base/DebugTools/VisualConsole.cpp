#include "VisualConsole.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "Base/InputManager.h"

#define S2DE_CONSOLE_DEFAULT_MODE VisualConsoleSizeMode::Fullscreen

namespace S2DE::Core::Debug
{
	std::vector<ConsoleBufferObject> VisualConsole::ConsoleBuffer;
	static std::uint32_t line_count = 0;

	VisualConsole::VisualConsole() : 
		m_showMessages(true),
		m_scroll_to_bottom(false), 
		m_free(Engine::isEditor()),
		m_sizemode(S2DE_CONSOLE_DEFAULT_MODE)
	{ 
		Logger::onPrintCallbacks.push_back(std::bind(&VisualConsole::Scroll, this));
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
		line_count++;

		if (line_count >= 6)
			line_count = 0;

		Line line = Line();
		line.alpha = 1.0f;
		line.text = ConsoleBuffer[(std::int32_t)ConsoleBuffer.size() - 1].text;

		m_linebuffer[line_count] = line;
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

	void VisualConsole::ToggleDraw()
	{
		m_draw = !m_draw;
		Core::Engine::GetInputManager()->LockWheel(m_draw);
	}

	void VisualConsole::Render()
	{
#if defined(_DEBUG) && defined(S2DE_DEBUG_RENDER_MODE)
		if (m_showMessages)
		{
			if (ImGui::Begin("ConsoleMessageUI", nullptr,
				ImGuiWindowFlags_::ImGuiWindowFlags_NoInputs |
				ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_::ImGuiWindowFlags_NoDecoration))
			{
				ImGui::SetWindowPos(ImVec2(0.0f, 10.0f));
				ImGui::SetWindowSize(ImVec2(Engine::GetGameWindow()->GetWidth(), 500.0f));
				for (std::int32_t i = 0; i <= (std::int32_t)line_count; i++)
				{
					Line& line = m_linebuffer[i];

					ImVec4 col = ImVec4(1.0f, 1.0f, 1.0f, line.alpha);

					if (strstr(line.text.c_str(), "[Error]"))
						col = ImColor(1.0f, 0.4f, 0.4f, line.alpha);
					else if (strstr(line.text.c_str(), "[Fatal]"))
						col = ImColor(1.0f, 0.0f, 0.0f, line.alpha);
					else if (strstr(line.text.c_str(), "[Warning]"))
						col = ImColor(0.8f, 0.8f, 0.0f, line.alpha);

					ImGui::PushStyleColor(ImGuiCol_Text, col);
					ImGui::TextUnformatted(line.text.c_str());
					ImGui::SameLine();
					ImGui::Text("| %d %d", i, line_count);
					ImGui::PopStyleColor();

					if (line.alpha > 0.0f)
						line.alpha -= 0.005f;
					else if (i == (std::int32_t)line_count && line.alpha <= 0.0f)
						line_count = 0;
					else if (line.alpha <= 0.0f && line_count > -1)
						line_count--;
				}
			}
			ImGui::End();
		}
#endif

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
			auto item = ConsoleBuffer[i];

			std::int32_t max_lines = 175 + Engine::GetGameWindow()->GetWidth() / 50;

			if (item.text.length() >= (std::uint32_t)max_lines)
			{
				for (std::int32_t i = 1; i <= std::int32_t(item.text.length() / max_lines); i++)
				{
					item.text.insert(max_lines * i, "\n");
				}
			}

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(item.color.x, item.color.y, item.color.z, item.color.w));
			ImGui::TextUnformatted(item.text.c_str());
			ImGui::PopStyleColor();
		}

		if (m_scroll_to_bottom)
		{
			ImGui::SetScrollHereY(1.0f);
		}

		m_scroll_to_bottom = false;
		ImGui::PopStyleVar();
		ImGui::EndChild();

		ImGui::End();
	}
}