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

		virtual void						  Render() override;										  
		inline GameObjects::GameObject*		  GetHandle() { return m_handle; }
		void								  Reset();
	private:
		GameObjects::GameObject*		m_handle;
		GameObjects::Components::Component*		m_component;
		std::string						m_select_object_name;
		bool							m_show_engine_object;
	};
}
