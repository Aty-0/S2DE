#pragma once
#include "Base/Main/Common.h"
#include "Graphics/ImGui_Window.h"

namespace S2DE
{
	class S2DE_API EditorRenderWindow : public ImGui_Window
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
