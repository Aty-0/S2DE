#include "EditorObjectInspector.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Base/GameWindow.h"

namespace S2DE
{
	EditorObjectInspector::EditorObjectInspector() : 
		m_handle(nullptr)
	{

	}

	EditorObjectInspector::~EditorObjectInspector()
	{

	}

	void EditorObjectInspector::Render()
	{
		if (!m_draw)
			return;

		ImGui::Begin("EditorObjectInspector", 0, ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);

		if (Engine::GetSceneManager()->GetScene() != nullptr)
		{
			if (ImGui::ListBoxHeader("", ImVec2(0.0f, (ImGui::GetWindowSize().y / 2) + 50.0f )))
			{
				for (SceneObjectStorage::iterator it = Engine::GetSceneManager()->GetScene()->GetStorage().begin();
					it != Engine::GetSceneManager()->GetScene()->GetStorage().end();
					it++)
				{
					if (it->second == nullptr)
						continue;

					if (ImGui::Selectable(it->first.first.c_str(), it->second->isSelected()))
					{
						if (m_handle == nullptr)
						{
							m_handle = it->second.get();
							m_handle->Select();
						}
						else if (m_handle != it->second.get())
						{
							m_handle->Unselect();
							m_handle = it->second.get();
						}
					}

				}
				ImGui::ListBoxFooter();
			}


			if (m_handle)
			{
				ImGui::Text("Name: %s ", m_handle->GetName().c_str());
				ImGui::Text("Type: %s ", m_handle->GetType().c_str());
				ImGui::Text("Prefix: %d ", m_handle->GetPrefix());
				ImGui::Text("UUID: %s ", m_handle->GetUUIDString().c_str());
				ImGui::Text("Visible: %d ", m_handle->isVisible());
				ImGui::Text("Enabled: %d ", m_handle->isEnabled());
				ImGui::Text("Position: X: %10.5f Y: %10.5f Z: %10.5f", m_handle->GetPosition().x, m_handle->GetPosition().y, m_handle->GetPosition().z);
				ImGui::Text("Rotation: X: %10.5f Y: %10.5f Z: %10.5f", m_handle->GetRotation().x, m_handle->GetRotation().y, m_handle->GetRotation().z);
				ImGui::Text("Scale: X: %10.5f Y: %10.5f Z: %10.5f", m_handle->GetScale().x, m_handle->GetScale().y, m_handle->GetScale().z);
			}
			ImGui::End();
		}
		
	}
}