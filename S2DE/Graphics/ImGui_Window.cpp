#include "ImGui_Window.h"

namespace S2DE
{
	ImGui_Window::ImGui_Window()
		: m_draw(false)
	{

	}

	ImGui_Window::~ImGui_Window()
	{

	}

	void ImGui_Window::TougleDraw()
	{ 
		m_draw = !m_draw; 
	}
}