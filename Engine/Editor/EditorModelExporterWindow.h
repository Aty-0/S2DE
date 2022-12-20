#pragma once
#include "Base/Main/Common.h"
#include "Render/ImGui_Window.h"

namespace S2DE::Editor
{
	class S2DE_API EditorModelExporterWindow : public Render::ImGui_Window
	{
	public:
		EditorModelExporterWindow();
		~EditorModelExporterWindow();

		void Render() override;
	};
}
