#pragma once
#include "Base/Main/Common.h"
#include "Render/Renderer.h"


//It's not thing from imgui library 
//It's just simple base for fast creating and then render it
//These windows creating in renderer

namespace S2DE::Render
{
	class S2DE_API ImGui_Window
	{
	public:
		ImGui_Window();
		virtual ~ImGui_Window();

		virtual void Render() = 0;
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