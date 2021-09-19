#pragma once
#include "Base/Main/Common.h"
#include "Graphics/Renderer.h"


namespace S2DE
{
	class S2DE_API Debug_Window
	{
	public:
		Debug_Window();
		~Debug_Window();

		virtual void Render() = 0;
		virtual void TougleDraw();
		inline bool  isDrawing() const { return m_draw; }

	protected:
		bool m_draw;
	};
}