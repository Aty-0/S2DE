#include "EditorObjectInspector.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Base/GameWindow.h"

namespace S2DE
{
	EditorObjectInspector::EditorObjectInspector()
	{

	}

	EditorObjectInspector::~EditorObjectInspector()
	{

	}

	void EditorObjectInspector::Render()
	{
		if (!m_draw)
			return;

		ImGui::Begin("EditorObjectInspector", 0,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);

		ImGui::SetWindowPos(ImVec2(0.0f, 25.0f));
		ImGui::SetWindowSize(ImVec2(300.0f, float(Engine::GetGameWindow()->GetHeight() / 2)));

		if (Engine::GetSceneManager()->GetScene() != nullptr)
		{
			ImGui::Text("TODO: Select object\nBasic object parameterts change");
			if (ImGui::ListBoxHeader("", ImVec2(0.0f, (ImGui::GetWindowSize().y / 2) + 50.0f )))
			{
				for (SceneObjectStorage::iterator it = Engine::GetSceneManager()->GetScene()->GetStorage().begin();
					it != Engine::GetSceneManager()->GetScene()->GetStorage().end();
					it++)
				{
					if (it->second == nullptr)
						continue;

					bool select = it->second->isSelected();
					if (ImGui::Selectable(it->first.first.c_str(), &select))
					{

					}

				}
				ImGui::ListBoxFooter();
			}



			ImGui::Text("TODO: Customed components ");
			ImGui::Text("Name: ");
			ImGui::Text("Type: ");
			ImGui::Text("Prefix: ");
			ImGui::Text("UUID: ");
			ImGui::Text("Visible: ");
			ImGui::Text("Enabled: ");
			ImGui::Text("Position: ");
			ImGui::Text("Rotation: ");
			ImGui::Text("Scale: ");

			ImGui::End();
		}
		
	}
}