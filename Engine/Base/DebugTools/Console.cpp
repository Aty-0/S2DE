#include "Console.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "Base/InputManager.h"

#define S2DE_CONSOLE_DEFAULT_MODE ConsoleSizeMode::Fullscreen

namespace S2DE::Core::Debug
{
	std::vector<console_buffer_object_t> Console::ConsoleBuffer;
	std::vector<console_command_t> Console::ConsoleCommandList;
	static std::uint32_t line_count = 0;

	Console::Console() :
		m_showMessages(true),
		m_scroll_to_bottom(false),
		m_free(Engine::isEditor()),
		m_sizemode(S2DE_CONSOLE_DEFAULT_MODE)
	{
		Logger::onPrintCallbacks.push_back(std::bind(&Console::Scroll, this));
		console_command_t clear = { "clear", []() { ConsoleBuffer.clear(); } };
		ConsoleCommandList.push_back(clear);
		console_command_t openLog = { "open log", []() { ShellExecute(0, 0, Logger::GetLogFileName().c_str(), 0, 0, SW_SHOW); } };
		ConsoleCommandList.push_back(openLog);
	}

	Console::~Console()
	{
		Clear();
	}

	void Console::Clear()
	{
		ConsoleCommandList.clear();
		ConsoleBuffer.clear();
	}

	void Console::Scroll()
	{
		m_scroll_to_bottom = true;
		/*
		line_count++;

		const uint8_t maxLineCount = 8;
		if (line_count >= maxLineCount)
			line_count = 0;

		Line line = Line();
		line.alpha = 1.0f;
		line.text = ConsoleBuffer[(std::int32_t)ConsoleBuffer.size() - 1].text;

		m_linebuffer[line_count] = line;
		*/
	}

	void Console::UpdateSizeMode()
	{
		switch (m_sizemode)
		{
		case ConsoleSizeMode::Default:
			ImGui::SetWindowSize("Console", ImVec2(float(Engine::GetGameWindow()->GetWidth() - 10), 400.0f));
			break;
		case ConsoleSizeMode::Fullscreen:
			ImGui::SetWindowSize("Console", ImVec2(float(Engine::GetGameWindow()->GetWidth() - 10), float(Engine::GetGameWindow()->GetHeight() - 32.0f)));
			break;
		case ConsoleSizeMode::Half:
			ImGui::SetWindowSize("Console", ImVec2(float(Engine::GetGameWindow()->GetWidth() / 2), float(Engine::GetGameWindow()->GetHeight() - 35.0f)));
			break;
		}
	}

	void Console::ToggleDraw()
	{
		m_draw = !m_draw;
		Core::Engine::GetInputManager()->LockWheel(m_draw);
	}

	void Console::EnterCommand(char* buff)
	{
		if (buff)
		{
			for (const auto command : ConsoleCommandList)
			{
				if (command.name == std::string(buff))
				{
					Logger::LogColored(DirectX::SimpleMath::Color(1.0f, 0.3f, 0.0f, 1.0f), "~ %s", command.name.c_str());

					if (command.func != nullptr)
					{
						command.func();
						return;
					}
				}
			}
			Logger::Error("~ No command with name %s", buff);
		}
		// no text entered
	}

	void Console::Render()
	{
		static bool textBoxUsed = false;
		// FIX ME:
		/*
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
		*/
		// Lock all control stuff if window is active
		/*Core::Engine::GetInputManager()->LockKeyboardControl(textBoxUsed);
		Core::Engine::GetInputManager()->LockMouseControl(textBoxUsed);
		Core::Engine::GetInputManager()->LockWheel(textBoxUsed);*/

		if (!m_draw)
			return;

		ImGui::Begin("Console", &m_draw,
			m_free == false ?
			ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar
			: 0);


		// Reset position if it's not free mode
		if (m_free == false)
		{
			UpdateSizeMode();
			ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
		}

		ImGui::SameLine();

		/*if (ImGui::Button("Clear"))
			ConsoleBuffer.clear();*/

		if (!Engine::isEditor())
		{
			ImGui::SameLine();
			ImGui::Checkbox("Free mode", &m_free);
			ImGui::SameLine();
		}

		if (!Engine::isEditor())
		{
			static std::int32_t mode = static_cast<std::int32_t>(S2DE_CONSOLE_DEFAULT_MODE);
			m_sizemode = static_cast<ConsoleSizeMode>(mode);
			ImGui::PushItemWidth(100);
			if (ImGui::SliderInt("Mode", &mode, 0, 2, "", 0))
				UpdateSizeMode();
		}

		static char buff[128] = "\0";
		textBoxUsed = ImGui::InputText("Enter command ", buff, (sizeof(buff) / sizeof(buff[0])), 0);
		if (Core::Engine::GetInputManager()->IsKeyPressed(Core::Other::KeyCode::KEY_RETURN))
		{
			EnterCommand(buff);
		}

		ImGui::SameLine();
		if (ImGui::Button("Enter"))
		{
			EnterCommand(buff);
		}

		// Text field 
		{
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
					for (std::int32_t j = 1; j <= std::int32_t(item.text.length() / max_lines); j++)
					{
						item.text.insert(max_lines * j, "\n");
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
		}

		ImGui::End();
	}

}