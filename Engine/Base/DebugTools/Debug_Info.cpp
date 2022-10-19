#include "Debug_Info.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Base/Utils/AppUsage.h"
#include "Base/GameWindow.h"

namespace S2DE::Core::Debug
{
	Debug_Info::Debug_Info()
	{

	}

	Debug_Info::~Debug_Info()
	{

	}

	void Debug_Info::Render() 
	{
		if (!m_draw)
			return;

		ImGui::Begin("Info Window", 0,
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMouseInputs);

		ImGui::SetWindowPos(ImVec2(Engine::GetGameWindow()->GetWidth() / 2 - ImGui::GetWindowSize().x, 
			/* Engine::GetGameWindow()->GetHeight() - */ ImGui::GetWindowSize().y));
		ImGui::SetWindowSize(ImVec2(550.0f, 300.0f));

		if (ImGui::BeginTable("##table1", 3))
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("Engine:");
			ImGui::Text("isEditor:%d\nMem:%d mb\nVsync:%d",
				Engine::isEditor(),
				GetUsedMem(),
				Engine::GetRenderer()->GetVsync());


			ImGui::TableSetColumnIndex(1);
			ImGui::Text("Engine Time:");

			ImGui::Text("%.3f ms/frame\n%d FPS\n%.3f Time",
				Engine::GetGameTime().GetDeltaTime(),
				Engine::GetGameTime().GetFPS(),
				Engine::GetGameTime().GetTime());


			ImGui::EndTable();
		}

		//ImGui::Separator();
		if (ImGui::BeginTable("##table2", 3))
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("Resource Manager:");
			ImGui::Text("Data name:%s", Engine::GetResourceManager().GetNameOfDataFolder().c_str());
			ImGui::Text("Loaded Texture's:%d\nLoaded Shader's:%d", 
				Engine::GetResourceManager().GetResourceCount<Render::FR::Texture>(),
				Engine::GetResourceManager().GetResourceCount<Render::FR::Shader>());
			

			ImGui::EndTable();
		}

		if (Engine::GetSceneManager()->GetScene() != nullptr)
		{
			if (ImGui::BeginTable("##table3", 3))
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Scene Statistics:");
				ImGui::Text("Objects:%d", Engine::GetSceneManager()->GetScene()->GetStorage().size());
				ImGui::Text("Name:%s", Engine::GetSceneManager()->GetScene()->GetName().c_str());

				ImGui::EndTable();
			}
		}
		ImGui::End();
	}
}