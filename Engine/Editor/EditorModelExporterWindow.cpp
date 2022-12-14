#include "EditorModelExporterWindow.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Base/GameWindow.h"
#include "Scene/SceneManager.h"
#include "GameObjects/Components/StaticMesh.h"
#include "Base/InputManager.h"

namespace S2DE::Editor
{
	EditorModelExporterWindow::EditorModelExporterWindow()
	{

	}

	EditorModelExporterWindow::~EditorModelExporterWindow()
	{

	}

	void EditorModelExporterWindow::Render()
	{
		if (!m_draw)
			return;

		ImGui::SetNextWindowSizeConstraints(ImVec2(300, 300), ImVec2(800, Core::Engine::GetGameWindow()->GetHeight()));

		ImGui::Begin("EditorModelExporterWindow", &m_draw, ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);
		{
			//Lock all control stuff if window is active
			Core::Engine::GetInputManager()->LockKeyboardControl(ImGui::IsWindowFocused(ImGuiFocusedFlags_::ImGuiFocusedFlags_ChildWindows));
			Core::Engine::GetInputManager()->LockMouseControl(ImGui::IsWindowFocused(ImGuiFocusedFlags_::ImGuiFocusedFlags_ChildWindows));
			if (Core::Engine::GetSceneManager()->GetScene() != nullptr)
			{
				static float position[3];
				static float rotation[3];
				static float scale[3];
				static char fileName[1024];

				ImGui::InputFloat3("Position", position);
				ImGui::InputFloat3("Rotation", rotation);
				ImGui::InputFloat3("Scale", scale);
				ImGui::InputText("File Name", fileName, sizeof(fileName));
				static bool useIndices = false;
				ImGui::Checkbox("useIndices", &useIndices);

				if (ImGui::Button("Load & Create New Object"))
				{
					auto meshGO = Scene::CreateGameObject<GameObjects::GameObject>(fileName, std::string(), 0, 
						DirectX::SimpleMath::Vector3(position[0], position[1], position[2]),
						DirectX::SimpleMath::Vector3(rotation[0], rotation[1], rotation[2]),
						DirectX::SimpleMath::Vector3(scale[0], scale[1], scale[2]));

					auto meshComponent = meshGO->CreateComponent<GameObjects::Components::StaticMesh>();
					meshComponent->UseIndices(useIndices);
					meshComponent->LoadMesh(fileName);
				}

			}
		}

		ImGui::End();
	}
}