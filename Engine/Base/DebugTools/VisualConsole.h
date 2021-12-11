#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/Logger.h"
#include "Graphics/Renderer.h"

namespace S2DE::Core::Debug
{
	enum class S2DE_API VisualConsoleSizeMode
	{
		Default = 0,
		Half,
		Fullscreen,
	};

	class S2DE_API VisualConsole
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

		//Set window size by VisualConsoleSizeMode
		void					UpdateSizeMode();
	private:
		bool					m_scroll_to_bottom;
		std::vector<std::string> 		m_buffer;
		bool					m_draw;
		bool					m_free;
		VisualConsoleSizeMode	m_sizemode;
	};
}