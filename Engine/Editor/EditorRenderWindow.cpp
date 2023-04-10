#include "EditorRenderWindow.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Base/GameWindow.h"

namespace S2DE::Editor
{
	EditorRenderWindow::EditorRenderWindow()
	{

	}

	EditorRenderWindow::~EditorRenderWindow()
	{

	}

	bool EditorRenderWindow::HandleWindowResize()
	{
		ImVec2 view = ImGui::GetContentRegionAvail();

		if (view.x != m_width || view.y != m_height)
		{
			if (view.x == 0 || view.y == 0)
			{
				// The window is too small or collapsed.
				return false;
			}

			m_width = view.x;
			m_height = view.y;

			Core::Engine::GetRenderer()->UpdateViewport();

			// The window state has been successfully changed.
			return true;
		}

		// The window state has not changed.
		return true;
	}

	inline float EditorRenderWindow::GetWindowWidth() const
	{
		return m_width;
	}

	inline float EditorRenderWindow::GetWindowHeight() const
	{
		return m_height;
	}

	void EditorRenderWindow::SetDefaultResolution()
	{
		ImGui::SetWindowSize("Render Window", ImVec2(float(Core::Engine::GetGameWindow()->GetWidth() / 2), 
			float(Core::Engine::GetGameWindow()->GetHeight() / 2)));
	}

	void EditorRenderWindow::PushRenderTexture(void* texture)
	{
		m_bufferdata = texture;
	}

	void EditorRenderWindow::Reset()
	{
		m_bufferdata = nullptr;
	}

	void EditorRenderWindow::Render()
	{
		if (m_draw)
		{
			ImGui::Begin("Render Window");

			m_width  = ImGui::GetWindowWidth();
			m_height = ImGui::GetWindowHeight();

			if(!HandleWindowResize())
			{
				ImGui::End();
				return; 
			}

			if (m_bufferdata != nullptr)
			{				
				const auto window = Core::Engine::GetGameWindow();
				ImGui::Image(m_bufferdata, ImVec2(window->GetWidth(), window->GetHeight()));
			}

			ImGui::End();
		}
	}

}