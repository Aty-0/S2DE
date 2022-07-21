#pragma once
#include "Base/Main/Common.h"
#include "Render/ImGui_Window.h"

namespace S2DE::Editor
{
	class S2DE_API EditorRenderWindow : public Render::ImGui_Window
	{
	public:
		EditorRenderWindow();
		~EditorRenderWindow();

		virtual void PushRenderTexture(void* texture);
		virtual void SetDefaultResolution();
		virtual void Render() override;
	private:
		void* m_bufferdata;
	};
}
