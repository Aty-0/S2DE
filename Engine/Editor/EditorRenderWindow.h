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

		[[nodiscard]] inline float GetWindowWidth() const;
		[[nodiscard]] inline float GetWindowHeight() const;
		
		bool		 HandleWindowResize();
		void		 Reset();
		virtual void PushRenderTexture(void* texture);
		virtual void SetDefaultResolution();
		void Render() override;
	private:
		void* m_bufferdata;
		float m_width;
		float m_height;
	};
}
