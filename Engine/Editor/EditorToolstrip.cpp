#include "EditorToolstrip.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Graphics/Renderer.h"

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

		m_inspector = reinterpret_cast<EditorObjectInspector*>(Core::Engine::GetRenderer()->GetImGui_Window("EditorObjectInspector"));
		S2DE_ASSERT(m_inspector);
		
		if (ImGui::BeginMainMenuBar()) 
		{
			if (ImGui::BeginMenu("File")) 
			{
				if (ImGui::MenuItem("Open"))
				{

				}

				if (ImGui::MenuItem("Save"))
				{

				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Object inspector"))
				{
					m_inspector->ToggleDraw();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit")) 
			{
				//TODO
				//Get selected object
				if (ImGui::MenuItem("Undo"))
				{

				}

				if (ImGui::MenuItem("Redo"))
				{

				}

				if (ImGui::MenuItem("Delete"))
				{
					if (m_inspector->GetHandle() != nullptr)
					{
						//Get object name from handle
						std::string handle_name = m_inspector->GetHandle()->GetName();
						//Reset handle in inspector 
						m_inspector->Reset();
						//Delete object
						Core::Engine::GetSceneManager()->GetScene()->Delete(handle_name);
					}
				}

				if (ImGui::MenuItem("Clone"))
				{
			
				}

				if (ImGui::MenuItem("Rename"))
				{

				}
			
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
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
					m_inspector->Reset();
					Core::Engine::GetSceneManager()->GetScene()->Clear();
				}

				if (ImGui::MenuItem("Toggle GameObject update"))
				{
					Core::Engine::GetSceneManager()->ToggleGameObjectUpdate();
				}

				if (ImGui::BeginMenu("Render"))
				{
					if (ImGui::MenuItem("Toggle objects visible"))
					{
						Core::Engine::GetSceneManager()->ToggleGameObjectVisibility();
					}

					if (ImGui::MenuItem("Toggle gizmos visible"))
					{

					}

					if (ImGui::MenuItem("Toggle debug gui visible"))
					{
						Core::Engine::GetSceneManager()->ToggleDebugGUIVisibility();
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

					//TODO
					//Open color picker window
					if (ImGui::MenuItem("Change background color"))
					{

					}

					ImGui::EndMenu();
				}


				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}