#include "ImGui_Window.h"

namespace S2DE::Render
{
	ImGui_Window::ImGui_Window()
		: m_draw(false)
	{

	}

	ImGui_Window::~ImGui_Window()
	{
		m_draw = false;
		m_name.clear();
	}

	void ImGui_Window::SetName(std::string name)
	{
		m_name = name;
	}

	void ImGui_Window::SetDrawState(bool draw)
	{
		m_draw = draw;
	}

	void ImGui_Window::ToggleDraw()
	{ 
		m_draw = !m_draw; 
	}
}