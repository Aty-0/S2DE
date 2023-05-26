#include "EditorModelExporterWindow.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "Base/InputManager.h"
#include "Scene/SceneManager.h"
#include "GameObjects/Components/StaticMesh.h"
#include "GameObjects/Components/Camera.h"
#include "Render/Renderer.h"

namespace S2DE::Editor
{
	EditorModelExporterWindow::EditorModelExporterWindow()
	{

	}

	EditorModelExporterWindow::~EditorModelExporterWindow()
	{

	}

	void EditorModelExporterWindow::Render(Render::Renderer* renderer)
	{
		if (!m_draw)
			return;
	
		ImGui::Begin("EditorModelExporterWindow", &m_draw, ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);
		{
			// TODO: Lock game input 

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

			if (ImGui::Button("Get camera position"))
			{
				const auto camera = Scene::GetObjectByName(GameObjects::Components::Camera::EngineCameraName);
				const auto pos = camera->GetTransform()->GetPosition();
				position[0] = pos.x;
				position[1] = pos.y;
				position[2] = pos.z;
			}

			if (ImGui::Button("Get camera rotation"))
			{
				const auto camera = Scene::GetObjectByName(GameObjects::Components::Camera::EngineCameraName);
				const auto rot = camera->GetTransform()->GetRotation();
				rotation[0] = rot.x;
				rotation[1] = rot.y;
				rotation[2] = rot.z;
			}


			if (ImGui::Button("Load & Create New Object"))
			{
				auto meshGO = Scene::CreateGameObject<GameObjects::GameObject>(fileName, std::string(), 0,
					Math::float3(position[0], position[1], position[2]),
					Math::float3(rotation[0], rotation[1], rotation[2]),
					Math::float3(scale[0], scale[1], scale[2]));

				auto meshComponent = meshGO->CreateComponent<GameObjects::Components::StaticMesh>();
				meshComponent->UseIndices(useIndices);
				meshComponent->LoadMesh(fileName);
			}

			
		}

		ImGui::End();
	}
}