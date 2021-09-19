#include "Debug_Window.h"

namespace S2DE
{
	Debug_Window::Debug_Window()
		: m_draw(false)
	{

	}

	Debug_Window::~Debug_Window()
	{

	}

	void Debug_Window::TougleDraw() 
	{ 
		m_draw = !m_draw; 
	}
}