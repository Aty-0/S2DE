#pragma once
#include "Base/Main/Common.h"
#include "Graphics/Renderer.h"


//It's not thing from imgui library it's just simple base class for
//classes where I use only ImGui and I need add it to render
//in renderer I add a std::vector<ImGui_Window> and i render all his items 

namespace S2DE
{
	class S2DE_API ImGui_Window
	{
	public:
		ImGui_Window();
		~ImGui_Window();

		virtual void Render() = 0;
		virtual void ToggleDraw();
		inline bool  isDrawing() const { return m_draw; }
		
	protected:
		bool		 m_draw;
	};
}