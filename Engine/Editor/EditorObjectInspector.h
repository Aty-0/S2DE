#pragma once
#include "Base/Main/Common.h"
#include "Render/ImGui_Window.h"

namespace S2DE::GameObjects
{
	class GameObject;
}

namespace S2DE::GameObjects::Components
{
	class Component;
}

namespace S2DE::Editor
{
	class S2DE_API EditorObjectInspector : public Render::ImGui_Window
	{
	public:
		EditorObjectInspector();
		~EditorObjectInspector();

		void						  Render(Render::Renderer* renderer) override;										  

		[[nodiscard]] inline GameObjects::GameObject*				GetSeletectedGameObject();
		[[nodiscard]] inline GameObjects::Components::Component*	GetSeletectedComponent();

		void								  Reset();
	private:
		GameObjects::GameObject*				m_selectedGameObject;
		GameObjects::Components::Component*		m_selectedComponent;

		// Some custom stuff
		float									m_uiElementsHeight;
		bool									m_showEngineGameObjects;
	};
}
