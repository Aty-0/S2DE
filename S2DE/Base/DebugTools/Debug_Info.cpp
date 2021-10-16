#include "Debug_Info.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Base/Utils/AppUsage.h"
#include "Base/GameWindow.h"

namespace S2DE
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

		ImGui::Begin("Debug info window", 0,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar  | 
			ImGuiWindowFlags_::ImGuiWindowFlags_NoMouseInputs);

		//OLD
		//ImGui::SetWindowPos(ImVec2(float(Engine::GetGameWindow()->GetWidth() - 550.0f), 0.0f));
		ImGui::SetWindowPos(ImVec2(0.0f, Engine::GetGameWindow()->GetHeight() -  ImGui::GetWindowSize().y));
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

			ImGui::Text("Game Window:\n");
			ImGui::Text("Widht:%d\nHeight:%d\nLeft:%d\nTop:%d\nisActive:%d\nisChild:%d\nFullscreen:%d\nShowCursor:%d",
				Engine::GetGameWindow()->GetWidth(),
				Engine::GetGameWindow()->GetHeight(),
				Engine::GetGameWindow()->GetLeft(),
				Engine::GetGameWindow()->GetTop(),
				Engine::GetGameWindow()->isActive(),
				Engine::GetGameWindow()->isChild(),
				Engine::GetGameWindow()->isFullscreen(),
				Engine::GetGameWindow()->isShowingCursor());

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("Resource Manager:");
			ImGui::Text("Data name:%s", Engine::GetResourceManager().GetNameOfData().c_str());
			ImGui::Text("Loaded Texture's:%d\nLoaded Shader's:%d", 
				Engine::GetResourceManager().GetResourceCount<Texture>(),
				Engine::GetResourceManager().GetResourceCount<Shader>());
			

			ImGui::EndTable();
		}

		if (Engine::GetSceneManager()->GetScene() != nullptr)
		{
			if (ImGui::BeginTable("##table3", 3))
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Scene Stats:");
				ImGui::Text("Objects:%d", Engine::GetSceneManager()->GetScene()->GetStorage().size());
				ImGui::Text("Name:%s", Engine::GetSceneManager()->GetScene()->GetName().c_str());

				ImGui::EndTable();
			}
		}

		ImGui::End();
	}
}