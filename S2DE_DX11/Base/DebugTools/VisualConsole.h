#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/Logger.h"
#include "Graphics/Renderer.h"

namespace S2DE
{
	enum class S2DE_DX11_API VisualConsoleSizeMode
	{
		Default = 0,
		Half,
		Fullscreen,
	};

	class S2DE_DX11_API VisualConsole
	{
	public:
		VisualConsole();
		~VisualConsole();

		//Switch visibility of console
		void					TougleDraw();
		//Add text to buffer
		void					Add(std::string text);
		//Clear text  buffer
		void					Clear();
		//Render console
		void					Render();

	private:
		bool					m_scroll_to_bottom;
		ImGuiTextBuffer			m_imgui_text_buffer;
		bool					m_draw;
		bool					m_free;
		VisualConsoleSizeMode	m_sizemode;
	};
}