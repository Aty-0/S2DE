#pragma once
#include "Base/Main/Common.h"
#include "GameObjects/Components/UI/UIDrawableComponent.h"

namespace S2DE::GameObjects::Components
{
	class Sprite;
}

namespace S2DE::GameObjects::Components::Editor
{
	class S2DE_API EditorCenterCursor : public UI::UIDrawableComponent
	{
	public:
		EditorCenterCursor();
		virtual ~EditorCenterCursor();

		virtual void OnRender(Render::Renderer* renderer) override;
		virtual void OnCreate() override;

	private:
		class GameObjects::Components::Sprite* m_sprite;
	};
}
