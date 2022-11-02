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

	void EditorToolStrip::Render()
	{
		if (!m_draw)
			return;

		if (ImGui::BeginMainMenuBar()) 
		{
			auto maincamera = Scene::GetObjectByName<GameObjects::GameObject>(S2DE_MAIN_CAMERA_NAME)->GetComponent<GameObjects::Components::Camera>();

			if (ImGui::BeginMenu("File")) 
			{
				if (ImGui::MenuItem("Open scene"))
				{
					Core::Engine::GetSceneManager()->LoadScene("");
				}

				if (ImGui::MenuItem("Save scene"))
				{
					Core::Engine::GetSceneManager()->SaveScene();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Draw object inspector"))
				{
					m_inspector = Core::Engine::GetRenderer()->GetImGui_Window<Editor::EditorObjectInspector*>("EditorObjectInspector");

					m_inspector->ToggleDraw();
				}

				if (ImGui::MenuItem("Draw GameObject's"))
				{
					Core::Engine::GetSceneManager()->ToggleGameObjectVisibility();
				}

				if (ImGui::MenuItem("Draw editor windows"))
				{
					Core::Engine::GetRenderer()->ToggleImGuiWindowsVisible();
				}

				if (ImGui::BeginMenu("Fill mode"))
				{
					if (ImGui::MenuItem("Wireframe"))
					{
						Core::Engine::GetRenderer()->SwitchFillMode(Render::RenderFillMode::Wireframe);
					}

					if (ImGui::MenuItem("Solid"))
					{
						Core::Engine::GetRenderer()->SwitchFillMode(Render::RenderFillMode::Solid);
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit")) 
			{
				if (ImGui::MenuItem("Undo"))
				{
					Logger::Log("Not implemented");
				}

				if (ImGui::MenuItem("Redo"))
				{
					Logger::Log("Not implemented");
				}

				if (ImGui::MenuItem("Delete"))
				{
					m_inspector = Core::Engine::GetRenderer()->GetImGui_Window<Editor::EditorObjectInspector*>("EditorObjectInspector");

					if (m_inspector->GetSeletectedGameObject() != nullptr)
					{
						//Get object name from handle
						std::string handle_name = m_inspector->GetSeletectedGameObject()->GetName();
						//Reset handle in inspector 
						m_inspector->Reset();
						//Delete object
						Core::Engine::GetSceneManager()->GetScene()->Delete(handle_name);
					}
				}

				if (ImGui::MenuItem("Clone"))
				{
					Logger::Log("Not implemented");				
				}

				if (ImGui::MenuItem("Rename"))
				{
					Logger::Log("Not implemented");				
				}
			
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				if (ImGui::BeginMenu("Create"))
				{
					DirectX::SimpleMath::Vector3 spawn_vec = DirectX::SimpleMath::Vector3::Zero;

					if (maincamera != nullptr)
						spawn_vec = maincamera->GetOwner()->GetTransform()->GetPosition();

					if (ImGui::MenuItem("GameObject"))
					{
						Scene::CreateGameObject<GameObjects::GameObject>("GameObject", "GameObject", 1, spawn_vec);
					}
					if (ImGui::MenuItem("Sprite"))
					{
						auto sprite = Scene::CreateGameObject<GameObjects::GameObject>("Sprite", "GameObject", 1, spawn_vec + DirectX::SimpleMath::Vector3(0,0,-1));
						sprite->CreateComponent<GameObjects::Components::Sprite>();
					}
					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Reload textures"))
				{
					Core::Engine::GetResourceManager().ReloadTextures();
					Core::Engine::GetSceneManager()->UpdateTextures();
				}

				if (ImGui::MenuItem("Reload shaders"))
				{
					Core::Engine::GetResourceManager().ReloadShaders();
					Core::Engine::GetSceneManager()->UpdateShaders();
				}

				if (ImGui::MenuItem("Clear scene"))
				{
					m_inspector = Core::Engine::GetRenderer()->GetImGui_Window<Editor::EditorObjectInspector*>("EditorObjectInspector");

					m_inspector->Reset();
					Core::Engine::GetSceneManager()->GetScene()->Clear();
				}

				if (ImGui::MenuItem("Toggle GameObject update"))
				{
					Core::Engine::GetSceneManager()->ToggleGameObjectUpdate();
				}

				if (ImGui::BeginMenu("Render"))
				{
					if (ImGui::MenuItem("Toggle Vsync"))
					{
						Core::Engine::GetRenderer()->SetVsync(!Core::Engine::GetRenderer()->GetVsync());
					}

					if (ImGui::MenuItem("Change background color"))
					{
						Core::Engine::GetRenderer()->GetImGui_Window<Render::ImGui_Window*>("EditorBgColorPicker")->ToggleDraw();
					}

					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (maincamera != nullptr)
			{
				ImGui::SetCursorPos(ImVec2((Core::Engine::GetGameWindow()->GetWidth() / 2) - 100,0));
				
				if (ImGui::Button("2D"))
				{
					maincamera->SetProjectionMode(GameObjects::Components::Camera::CameraProjectionMode::Orthographics);
				}

				if (ImGui::Button("3D"))
				{
					maincamera->SetProjectionMode(GameObjects::Components::Camera::CameraProjectionMode::Perspective);
				}

				if (maincamera->GetProjectionMode() == GameObjects::Components::Camera::CameraProjectionMode::Perspective)
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