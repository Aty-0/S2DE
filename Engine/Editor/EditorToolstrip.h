#pragma once
#include "Base/Main/Common.h"
#include "Editor/EditorObjectInspector.h"
#include "Render/ImGui_Window.h"

namespace S2DE::Editor
{
	class S2DE_API EditorToolStrip : public Render::ImGui_Window
	{
	public:
		EditorToolStrip();
		~EditorToolStrip();
		void Render(Render::Renderer* renderer) override;
	private:
		EditorObjectInspector* m_inspector;

	};
}
