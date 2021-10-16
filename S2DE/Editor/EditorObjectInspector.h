#pragma once
#include "Base/Main/Common.h"
#include "Graphics/ImGui_Window.h"

namespace S2DE
{
	class S2DE_API EditorObjectInspector : public ImGui_Window
	{
	public:
		EditorObjectInspector();
		~EditorObjectInspector();

		virtual void Render() override;
	private:

	};
}
