#include "EditorToolstrip.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "Base/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Render/Renderer.h"

#include "GameObjects/Components/Camera.h"
#include "GameObjects/Components/Sprite.h"

#include "Editor/EditorColorPicker.h"
#include "Base/Utils/SerializeUtils.h"

#include <sstream>

namespace S2DE::Editor
{
	EditorToolStrip::EditorToolStrip() 
	{

	}

	EditorToolStrip::~EditorToolStrip()
	{

	}

	void EditorToolStrip::Render(Render::Renderer* renderer)
	{
		if (!m_draw)
			return;

		static const auto sceneManager = Scene::SceneManager::GetInstance();
		static const auto window = Core::GameWindow::GetInstance();

		if (ImGui::BeginMainMenuBar()) 
		{
			auto maincamera = sceneManager->GetObjectByName<GameObjects::GameObject>(GameObjects::Components::Camera::EngineCameraName)->GetComponent<GameObjects::Components::Camera>();

			if (ImGui::BeginMenu("File")) 
			{
				if (ImGui::MenuItem("Open scene"))
				{
					// TODO: Open dialog simple dialog 
					sceneManager->LoadScene("");
				}

				if (ImGui::MenuItem("Save scene"))
				{
					sceneManager->SaveScene();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Draw object inspector"))
				{
					m_inspector = renderer->GetImGui_Window<Editor::EditorObjectInspector*>("EditorObjectInspector");

					m_inspector->ToggleDraw();
				}

				if (ImGui::MenuItem("Draw editor windows"))
				{
					renderer->ToggleImGuiWindowsVisible();
				}

				if (ImGui::BeginMenu("Fill mode"))
				{
					if (ImGui::MenuItem("Wireframe"))
					{
						// TODO
					}

					if (ImGui::MenuItem("Solid"))
					{
						// TODO
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit")) 
			{
				if (ImGui::MenuItem("Undo"))
				{
					Core::Utils::Logger::Log("Not implemented");
				}

				if (ImGui::MenuItem("Redo"))
				{
					Core::Utils::Logger::Log("Not implemented");
				}

				if (ImGui::MenuItem("Delete"))
				{
					m_inspector = renderer->GetImGui_Window<Editor::EditorObjectInspector*>("EditorObjectInspector");

					if (m_inspector->GetSeletectedGameObject() != nullptr)
					{
						// Get object name from handle
						std::string handle_name = m_inspector->GetSeletectedGameObject()->GetName();
						// Reset handle in inspector 
						m_inspector->Reset();
						// Delete object
						sceneManager->GetScene()->Delete(handle_name);
					}
				}

				if (ImGui::MenuItem("Clone"))
				{
					Core::Utils::Logger::Log("Not implemented");
				}

				if (ImGui::MenuItem("Rename"))
				{
					Core::Utils::Logger::Log("Not implemented");
				}
			
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				if (ImGui::BeginMenu("Create"))
				{
					Math::float3 spawn_vec = Math::float3::Zero;

					if (maincamera != nullptr)
						spawn_vec = maincamera->GetOwner()->GetTransform()->GetPosition();

					if (ImGui::MenuItem("GameObject"))
					{
						sceneManager->CreateGameObject<GameObjects::GameObject>("GameObject", "GameObject", 1, spawn_vec);
					}
					if (ImGui::MenuItem("Sprite"))
					{
						auto sprite = sceneManager->CreateGameObject<GameObjects::GameObject>("Sprite", "GameObject", 1, spawn_vec + Math::float3(0,0,-1));
						sprite->CreateComponent<GameObjects::Components::Sprite>();
					}
					ImGui::EndMenu();
				}

				static const auto resourceManager = Core::Resources::ResourceManager::GetInstance();

				if (ImGui::MenuItem("Reload textures"))
				{
					resourceManager->ReloadTextures();
					sceneManager->UpdateTextures();
				}

				if (ImGui::MenuItem("Reload shaders"))
				{
					resourceManager->ReloadShaders();
					sceneManager->UpdateShaders();
				}

				if (ImGui::MenuItem("Clear scene"))
				{
					m_inspector = renderer->GetImGui_Window<Editor::EditorObjectInspector*>("EditorObjectInspector");

					m_inspector->Reset();
					sceneManager->GetScene()->Clear();
				}

				if (ImGui::MenuItem("Toggle GameObject update"))
				{
					sceneManager->ToggleGameObjectUpdating();
				}

				if (ImGui::BeginMenu("Render"))
				{
					if (ImGui::MenuItem("Toggle Vsync"))
					{
						renderer->SetVsync(!renderer->GetVsync());
					}

					if (ImGui::MenuItem("Change background color"))
					{
						renderer->GetImGui_Window<Render::ImGui_Window*>("EditorBgColorPicker")->ToggleDraw();
					}

					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (maincamera != nullptr)
			{
				ImGui::SetCursorPos(ImVec2((window->GetWidth() / 2) - 100,0));
				
				if (ImGui::Button("2D"))
				{
					maincamera->SetProjectionMode(GameObjects::Components::Camera::ProjectionMode::Orthographics);
				}

				if (ImGui::Button("3D"))
				{
					maincamera->SetProjectionMode(GameObjects::Components::Camera::ProjectionMode::Perspective);
				}

				if (maincamera->GetProjectionMode() == GameObjects::Components::Camera::ProjectionMode::Perspective)
				{
					static float fov = maincamera->GetFov();
					ImGui::PushItemWidth(100);
					if (ImGui::SliderFloat("FOV", &fov, 10.0f, 110.0f))
					{
						maincamera->SetFov(fov);
					}

					ImGui::Text("Speed boost %.3f x", maincamera->GetSpeedBoost());
				}
				else
				{
					static float zoom = maincamera->GetZoom();
					ImGui::PushItemWidth(100);
					if (ImGui::SliderFloat("Zoom", &zoom, 0.01f, 0.13f))
					{
						maincamera->SetZoom(zoom);
					}

					ImGui::Text("Speed boost %.3f x", maincamera->GetSpeedBoost());
				}

			}

			ImGui::EndMainMenuBar();
		}
	}
}