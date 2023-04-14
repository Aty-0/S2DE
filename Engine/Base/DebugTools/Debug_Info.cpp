#include "Debug_Info.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Base/Utils/AppUsage.h"
#include "Base/GameWindow.h"
#include "Render/LightGlobals.h"
#include "Render/CB.h"
#include "Render/Texture.h"
#include "Render/Shader.h"

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
		static ImVec4 debugInfoDefaultColor = ImColor(1.0f, 0.0f, 0.0f, 1.0f);

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

		ImGui::PushStyleColor(ImGuiCol_Text, debugInfoDefaultColor);

		ImGui::SetWindowPos(ImVec2(Engine::GetGameWindow()->GetWidth() - ImGui::GetWindowSize().x, ImGui::GetWindowSize().y));
		
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

		if (ImGui::BeginTable("##table2", 3))
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("Resources:");
			ImGui::Text("Texture:%d\nShader:%d\nMesh:%d", 
				Engine::GetResourceManager().GetResourceCount<Render::Texture>(),
				Engine::GetResourceManager().GetResourceCount<Render::Shader>(),
				Engine::GetResourceManager().GetResourceCount<Render::Mesh>()); 
			

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
				ImGui::Text("LightCount:%d Light limit:%d", Render::LightGlobals::GetLightCount(), Render::CB::MAX_LIGHT_COUNT);

				ImGui::EndTable();
			}
		}
		ImGui::PopStyleColor();

		ImGui::End();
	}
}