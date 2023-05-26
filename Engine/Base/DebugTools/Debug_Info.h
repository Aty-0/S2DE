#pragma once
#include "Base/Main/Common.h"

namespace S2DE::GameObjects::Components::UI
{
	class UIText;
}

namespace S2DE::Core::Debug
{
	class S2DE_API Debug_Info : public Core::Utils::Singleton<Debug_Info>
	{
	public:
		Debug_Info();
		~Debug_Info();

		void	ToggleDraw();
		void	CreateDbgTexts();
		void	UpdateTexts();

		void	UpdateTextPos();
	private:
		bool	m_draw;
		std::vector<S2DE::GameObjects::Components::UI::UIText*> m_texts;

	public:
		const float		  GlobalHeight = 38.0f;
	};

}