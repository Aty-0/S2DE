#pragma once
#include "Base/Main/Common.h"
#include "Graphics/ImGui_Window.h"
#include "Math/Color.h"


namespace S2DE::Editor
{
	class EditorColorPicker : public Render::ImGui_Window
	{
	public:
		EditorColorPicker();
		~EditorColorPicker();

		void		 SetColorRawFormat(const float* color);
		void		 SetColor(Math::Color<float>& color);

		virtual void Render() override;

	private:
		Math::Color <float>	 m_color;
		float				 m_raw_color[4];
	};
}
