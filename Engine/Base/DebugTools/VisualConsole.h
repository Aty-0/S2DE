#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/Logger.h"
#include "Render/Renderer.h"
#include "Render/ImGui_Window.h"

namespace S2DE::Core::Debug
{
	enum class S2DE_API VisualConsoleSizeMode
	{
		Default = 0,
		Half,
		Fullscreen,
	};


	class S2DE_API VisualConsole : public Render::ImGui_Window
	{
	public:
		static std::vector<std::string> ConsoleBuffer;

		VisualConsole();
		~VisualConsole();

		//Clear text  buffer
		void					Clear();
		//Render console
		virtual void			Render() override;

		void					Scroll();

		//Set window size by VisualConsoleSizeMode
		void					UpdateSizeMode();

	private:
		bool					m_showMessages = false;
		bool					m_scroll_to_bottom;
		bool					m_free;
		VisualConsoleSizeMode	m_sizemode;

		struct Line
		{
			float alpha;
			std::string text;
		};

		Line m_linebuffer[8];
	};
}