#pragma once
#include "Base/Main/Common.h"
#include "Graphics/ImGui_Window.h"

namespace S2DE
{
	class S2DE_API EditorToolStrip : public ImGui_Window
	{
	public:
		EditorToolStrip();
		~EditorToolStrip();
		virtual void Render() override;
	private:

	};
}
