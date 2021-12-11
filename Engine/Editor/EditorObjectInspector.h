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

		virtual void	  Render() override;

		void			  Reset();
		inline class GameObjects::GameObject* GetHandle() { return m_handle; }
	private:
		class GameObjects::GameObject* m_handle;
		std::string m_select_object_name;
	};
}
