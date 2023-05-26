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
		Core::Utils::Logger::onPrintCallbacks.push_back(std::bind(&Console::Scroll, this));
		console_command_t clear = { "clear", []() { ConsoleBuffer.clear(); } };
		ConsoleCommandList.push_back(clear);
		console_command_t openLog = { "open log", []() { ShellExecute(0, 0, Core::Utils::Logger::GetLogFileName().c_str(), 0, 0, SW_SHOW); } };
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
	}

	void Console::UpdateSizeMode()
	{
		static const auto window = Core::GameWindow::GetInstance();

		switch (m_sizemode)
		{
		case ConsoleSizeMode::Default:
			ImGui::SetWindowSize("Console", ImVec2(float(window->GetWidth() - 10), 400.0f));
			break;
		case ConsoleSizeMode::Fullscreen:
			ImGui::SetWindowSize("Console", ImVec2(float(window->GetWidth() - 10), float(window->GetHeight() - 32.0f)));
			break;
		case ConsoleSizeMode::Half:
			ImGui::SetWindowSize("Console", ImVec2(float(window->GetWidth() / 2), float(window->GetHeight() - 35.0f)));
			break;
		}
	}

	void Console::ToggleDraw()
	{
		m_draw = !m_draw;
		static const auto inputManager = Core::InputManager::GetInstance();
		inputManager->LockWheel(m_draw);
	}

	void Console::EnterCommand(char* buff)
	{
		if (buff)
		{
			for (const auto command : ConsoleCommandList)
			{
				if (command.name == std::string(buff))
				{
					Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(1.0f, 0.3f, 0.0f, 1.0f), "~ %s", command.name.c_str());

					if (command.func != nullptr)
					{
						command.func();
						return;
					}
				}
			}
			Core::Utils::Logger::Error("~ No command with name %s", buff);
		}
		// no text entered
	}

	void Console::Render(Render::Renderer* renderer)
	{
		if (!m_draw)
			return;

		ImGui::Begin("Console", &m_draw,
			m_free == false ?
			ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoResize 
			: 0);
		static const auto window = Core::GameWindow::GetInstance();
		// TODO: size offset 
		ImGui::BeginChild("scrolling", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() / 1.125f), false, 
			ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysVerticalScrollbar |
			ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysHorizontalScrollbar);
		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

		for (std::int32_t i = 0; i < static_cast<std::int32_t>(ConsoleBuffer.size()); i++)
		{
			auto item = ConsoleBuffer[i];

			static const auto window = Core::GameWindow::GetInstance();
			const auto max_lines = 175 + window->GetWidth() / 50;

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

		static bool textBoxUsed = false;
		static char buff[128] = "\0";
		textBoxUsed = ImGui::InputText("Enter command ", buff, (sizeof(buff) / sizeof(buff[0])), 0);
		static const auto inputManager = Core::InputManager::GetInstance();
		if (inputManager->IsKeyPressed(Core::Other::KeyCode::KEY_RETURN))
		{
			EnterCommand(buff);
		}

		ImGui::SameLine();
		if (ImGui::Button("Enter"))
		{
			EnterCommand(buff);
		}

		if (!Engine::isEditor())
		{
			ImGui::SameLine();
			if (ImGui::Checkbox("Free mode", &m_free))
			{
				UpdateSizeMode();
				ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
			}
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


		ImGui::End();
	}

}