#include "EditorRenderWindow.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Base/GameWindow.h"

namespace S2DE
{
	EditorRenderWindow::EditorRenderWindow()
	{

	}

	EditorRenderWindow::~EditorRenderWindow()
	{

	}

	void EditorRenderWindow::SetDefaultResolution()
	{
		ImGui::SetWindowSize("EditorRenderWindow", ImVec2(Engine::GetGameWindow()->GetWidth() / 2, Engine::GetGameWindow()->GetHeight() / 2));
	}

	void EditorRenderWindow::PushRenderTexture(void* texture)
	{
		m_bufferdata = texture;
	}

	void EditorRenderWindow::Render()
	{
		if (!m_draw)
			return;

		ImGui::Begin("EditorRenderWindow", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoDecoration);
		ImGui::Image((void*)m_bufferdata, ImVec2((float)Engine::GetGameWindow()->GetWidth(), (float)Engine::GetGameWindow()->GetHeight()));
		ImGui::End();

	}
}