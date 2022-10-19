#include "EditorObjectInspector.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Base/InputManager.h"
#include "Scene/SceneManager.h"
#include "Base/GameWindow.h"

#include <boost/range/adaptor/reversed.hpp>

namespace S2DE::Editor
{
	EditorObjectInspector::EditorObjectInspector() : 
		m_handle(nullptr),
		m_show_engine_object(false)
	{

	}

	EditorObjectInspector::~EditorObjectInspector()
	{
		
	}

	void EditorObjectInspector::Reset()
	{
		if(m_handle != nullptr)
			m_handle->Unselect();

		m_handle = nullptr;
		m_select_object_name.clear();
	}

	void EditorObjectInspector::Render()
	{
		if (!m_draw)
			return;

		ImGui::SetNextWindowSizeConstraints(ImVec2(300, 300), ImVec2(800, Core::Engine::GetGameWindow()->GetHeight()));

		ImGui::Begin("Scene object inspector", &m_draw, ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);

		//Lock all control stuff if window is active
		Core::Engine::GetInputManager()->LockKeyboardControl(ImGui::IsWindowFocused(ImGuiFocusedFlags_::ImGuiFocusedFlags_ChildWindows));
		Core::Engine::GetInputManager()->LockMouseControl(ImGui::IsWindowFocused(ImGuiFocusedFlags_::ImGuiFocusedFlags_ChildWindows));


		if (Core::Engine::GetSceneManager()->GetScene() != nullptr)
		{
			if (ImGui::Checkbox("Show Engine Objects", &m_show_engine_object))
				Reset();

			if (m_handle != nullptr)
				ImGui::Text("Selected:%s", m_handle->GetName().c_str());

			ImGui::Spacing();

			if (ImGui::ListBoxHeader("1", ImVec2(ImGui::GetWindowSize().x, 300)))
			{
				for (const auto& object : boost::adaptors::reverse(Core::Engine::GetSceneManager()->GetScene()->GetStorage()))
				{
					//If iterator is empty we skip it
					if (object.second == nullptr)
						continue;
					
					//If we don't need to show objects with ENGINE prefix we skip that object
					if (m_show_engine_object == false && object.second->GetPrefix() == -1)
						continue;

					if (ImGui::Selectable(object.first.first.c_str(), object.second->isSelected()))
					{
						if (m_handle == nullptr)
						{
							m_select_object_name = object.first.first;
							m_handle = object.second.get();
							m_handle->Select();
						}
						else if (m_handle != object.second.get())
						{
							//Unselect previous object and select new object
							m_handle->Unselect();
							m_select_object_name = object.first.first;
							m_handle = object.second.get();
							m_handle->Select();
						}
					}
				}

				ImGui::ListBoxFooter();
			}
	
			ImGui::End();
		}
	}
}