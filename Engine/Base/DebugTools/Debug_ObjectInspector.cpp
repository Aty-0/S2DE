#include "Debug_ObjectInspector.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

namespace S2DE::Core::Debug
{
	Debug_ObjectInspector::Debug_ObjectInspector()
	{

	}

	Debug_ObjectInspector::~Debug_ObjectInspector()
	{

	}

	void Debug_ObjectInspector::Render()
	{
		if (!m_draw)
			return;

		ImGui::Begin("Debug Object Inspector", 0,
			ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);
	


		ImGui::End();
	}
}