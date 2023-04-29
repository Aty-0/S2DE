#pragma once
#include "Base/Main/Common.h"
#include "Render/ImGui_Window.h"

namespace S2DE::GameObjects::Components::UI
{
	class UIText;
}

namespace S2DE::Render
{
	class Renderer;
}

namespace S2DE::Core::Debug
{
	class S2DE_API Debug_Info
	{
	public:
		Debug_Info();
		~Debug_Info();

		void	ToggleDraw();
		void	CreateDbgTexts();
		void	UpdateTexts();

	private:
		bool	m_draw;
		std::vector<S2DE::GameObjects::Components::UI::UIText*> m_texts;

	public:
		static Debug_Info debugInfoTool;
	};

}