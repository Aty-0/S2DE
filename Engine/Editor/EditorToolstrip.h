#pragma once
#include "Base/Main/Common.h"
#include "Graphics/ImGui_Window.h"
#include "Editor/EditorObjectInspector.h"

namespace S2DE::Editor
{
	class S2DE_API EditorToolStrip : public Render::ImGui_Window
	{
	public:
		EditorToolStrip();
		~EditorToolStrip();
		virtual void Render() override;
	private:
		EditorObjectInspector* m_inspector;

	};
}
