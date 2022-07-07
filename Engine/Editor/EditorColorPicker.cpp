#include "EditorColorPicker.h"


namespace S2DE::Editor
{
	EditorColorPicker::EditorColorPicker() 
	{		 
		m_raw_color[0] = 0.0f;
		m_raw_color[1] = 0.0f;
		m_raw_color[2] = 0.0f;
		m_raw_color[3] = 0.0f;
	}

	EditorColorPicker::~EditorColorPicker()
	{

	}

	void EditorColorPicker::SetColorRawFormat(const float* color)
	{
		color = m_raw_color;
	}

	void EditorColorPicker::SetColor(Math::Color<float>& color)
	{
		color = m_color;
	}

	void EditorColorPicker::Render()
	{
		if (!m_draw)
			return;

		ImGui::Begin(std::string(m_name + " color picker").c_str(), &m_draw);
		ImGui::ColorPicker4("", m_raw_color);
		ImGui::End();

		m_color.r = m_raw_color[0];
		m_color.g = m_raw_color[1];
		m_color.b = m_raw_color[2];
		m_color.a = m_raw_color[3];

	}
}