#include "EditorToolstrip.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Scene/SceneManager.h"

namespace S2DE
{
	EditorToolStrip::EditorToolStrip() 
	{

	}

	EditorToolStrip::~EditorToolStrip()
	{

	}

	void EditorToolStrip::Render()
	{
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
					Engine::GetResourceManager().ReloadTextures();
				}

				if (ImGui::MenuItem("Reload shaders"))
				{
					Engine::GetResourceManager().ReloadShaders();
					Engine::GetSceneManager()->UpdateShaders();
				}

				if (ImGui::MenuItem("Clear scene"))
				{
					Engine::GetSceneManager()->GetScene()->Clear();
				}

				if (ImGui::MenuItem("Toggle GameObject update"))
				{
					Engine::GetSceneManager()->ToggleGameObjectUpdate();
				}

				if (ImGui::BeginMenu("Render"))
				{
					if (ImGui::MenuItem("Toggle objects visible"))
					{
						Engine::GetSceneManager()->ToggleGameObjectVisibility();
					}

					if (ImGui::MenuItem("Toggle gizmos visible"))
					{

					}

					if (ImGui::MenuItem("Toggle debug gui visible"))
					{
						Engine::GetSceneManager()->ToggleDebugGUIVisibility();
					}

					if (ImGui::BeginMenu("Fill mode"))
					{
						if (ImGui::MenuItem("Wireframe"))
						{
							Engine::GetRenderer()->SwitchFillMode(RenderFillMode::Wireframe);
						}

						if (ImGui::MenuItem("Solid"))
						{
							Engine::GetRenderer()->SwitchFillMode(RenderFillMode::Solid);
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

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("About"))
				{

				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}
}