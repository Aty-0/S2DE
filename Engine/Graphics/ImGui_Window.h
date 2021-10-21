#pragma once
#include "Base/Main/Common.h"
#include "Graphics/Renderer.h"


//It's not thing from imgui library 
//It's just simple base for fast creating and then render it
//These windows creating in renderer

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