#include "EditorObjectInspector.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Base/GameWindow.h"

namespace S2DE::Editor
{
	EditorObjectInspector::EditorObjectInspector() : 
		m_handle(nullptr)
	{

	}

	EditorObjectInspector::~EditorObjectInspector()
	{

	}

	void EditorObjectInspector::Reset()
	{
		m_handle = nullptr;
		m_select_object_name.clear();
	}

	void EditorObjectInspector::Render()
	{
		if (!m_draw)
			return;

		ImGui::Begin("EditorObjectInspector", &m_draw, ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);

		if (Core::Engine::GetSceneManager()->GetScene() != nullptr)
		{
			if (ImGui::ListBoxHeader("", ImVec2(0.0f, (ImGui::GetWindowSize().y / 2) + 50.0f )))
			{
				for (Scene::SceneObjectStorage::iterator it = Core::Engine::GetSceneManager()->GetScene()->GetStorage().begin();
					it != Core::Engine::GetSceneManager()->GetScene()->GetStorage().end();
					it++)
				{
					//if iterator is empty we try to skip it
					if (it == Core::Engine::GetSceneManager()->GetScene()->GetStorage().end())
						continue;

					if (ImGui::Selectable(it->first.first.c_str(), it->second->isSelected()))
					{
						if (m_handle == nullptr)
						{
							m_select_object_name = it->first.first;
							//Get object 
							m_handle = it->second.get();
							//Select it 
							m_handle->Select();
						}
						else if (m_handle != it->second.get())
						{
							//Unselect previous object
							m_handle->Unselect();
							//Get new object
							m_handle = it->second.get();

							m_select_object_name = it->first.first;
						}
					}

				}
				ImGui::ListBoxFooter();
			}


			if (m_handle != nullptr)
			{
				ImGui::Separator();
				ImGui::Text("Name: %s ", m_handle->GetName().c_str());
				ImGui::Text("Type: %s ", m_handle->GetType().c_str());
				ImGui::Text("Prefix: %d ", m_handle->GetPrefix());
				ImGui::Text("UUID: %s ", m_handle->GetUUIDString().c_str());
				ImGui::Separator();
				ImGui::Text("Visible: %d ", m_handle->isVisible());
				ImGui::Text("Enabled: %d ", m_handle->isEnabled());
				ImGui::Separator();
				ImGui::Text("Position: X: %10.5f Y: %10.5f Z: %10.5f", m_handle->GetPosition().x, m_handle->GetPosition().y, m_handle->GetPosition().z);
				ImGui::Text("Rotation: X: %10.5f Y: %10.5f Z: %10.5f", m_handle->GetRotation().x, m_handle->GetRotation().y, m_handle->GetRotation().z);
				ImGui::Text("Scale: X: %10.5f Y: %10.5f Z: %10.5f", m_handle->GetScale().x, m_handle->GetScale().y, m_handle->GetScale().z);
				ImGui::Separator();
			}
			ImGui::End();
		}
		
	}
}