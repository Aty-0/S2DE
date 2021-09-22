#include "EditorToolstrip.h"


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

				}

				if (ImGui::MenuItem("Reload shaders"))
				{

				}

				if (ImGui::MenuItem("Clear scene"))
				{

				}

				if (ImGui::BeginMenu("Render"))
				{
					if (ImGui::MenuItem("Tougle objects visible"))
					{

					}

					if (ImGui::MenuItem("Tougle gizmos visible"))
					{

					}

					if (ImGui::BeginMenu("Fill mode"))
					{
						if (ImGui::MenuItem("Wireframe"))
						{

						}

						if (ImGui::MenuItem("Solid"))
						{

						}

						ImGui::EndMenu();
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