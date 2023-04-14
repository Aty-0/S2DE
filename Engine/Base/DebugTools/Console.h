#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/Logger.h"
#include "Render/Renderer.h"
#include "Render/ImGui_Window.h"

namespace S2DE::Core::Debug
{
	enum class S2DE_API ConsoleSizeMode
	{
		Default = 0,
		Half,
		Fullscreen,
	};
	// TODO: Better impl of commands
	typedef struct ConsoleCommand
	{
		std::string name;
		std::function<void()> func;

	} console_command_t;

	typedef struct ConsoleBufferObject
	{
		std::string text;
		DirectX::SimpleMath::Color color;
	} console_buffer_object_t;

	class S2DE_API Console : public Render::ImGui_Window
	{
	public:
		static std::vector<console_buffer_object_t> ConsoleBuffer;
		static std::vector<console_command_t> ConsoleCommandList;

		Console();
		~Console();

		// Clear text buffer
		void Clear();
		
		void Render() override;
		void Scroll();

		// Set window size
		void UpdateSizeMode();

		void ToggleDraw() override;
		void EnterCommand(char* buff);
	private:
		bool					m_showMessages;
		bool					m_scroll_to_bottom;
		bool					m_free;
		ConsoleSizeMode			m_sizemode;

		
	};
}