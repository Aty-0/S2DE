#pragma once
#include "Base/Main/Common.h"
#include "Graphics/ImGui_Window.h"

namespace S2DE::GameObjects
{
	class GameObject;
}

namespace S2DE::Editor
{
	class S2DE_API EditorObjectInspector : public Render::ImGui_Window
	{
	public:
		EditorObjectInspector();
		~EditorObjectInspector();

		virtual void						  Render() override;										  
		inline class GameObjects::GameObject* GetHandle() { return m_handle; }
		void								  Reset();
	private:
		class GameObjects::GameObject*	m_handle;
		std::string						m_select_object_name;
		bool							m_show_engine_object;
	};
}
