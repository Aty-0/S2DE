#pragma once
#include "Base/Main/Common.h"

// Basic template for parsing our windows as objects

namespace S2DE::Render
{
	class Renderer;
	class S2DE_API ImGui_Window
	{
	public:
		ImGui_Window();
		virtual ~ImGui_Window();

		virtual void Render(Render::Renderer* renderer) = 0;
		virtual void ToggleDraw();
		virtual void SetDrawState(bool draw);
		virtual void SetName(std::string name);
		inline std::string GetName() { return m_name; }
		inline bool  isDrawing() const { return m_draw; }
		

	protected:
		bool		 m_draw;
		std::string	 m_name;
	};
}