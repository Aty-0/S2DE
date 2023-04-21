#pragma once
#include "GameObjects/Components/UI/UIDrawableComponent.h"

namespace S2DE::GameObjects::Components
{
	class Sprite;
}

namespace S2DE::Render
{
	class Font;
}

namespace S2DE::Render
{
	class Shader;

	template<typename T>
	class VertexBuffer;
}

namespace S2DE::GameObjects::Components::UI
{
	class S2DE_API UIText : public GameObjects::Components::UI::UIDrawableComponent
	{
	public:
		UIText();
		virtual ~UIText();

		void OnRender(Render::Renderer* renderer) override;

		void SetText(const char* text, ...);
		void SetText(std::string text);

		// Todo: load font
		void SetFont(Render::Font* font);

		void Rebuild();

		void OnCreate() override;

	private:
		Render::Shader* m_shader;
		std::vector<Render::VertexBuffer<Render::Vertex>*> m_vertexBuffers;
		std::string m_text;
		Render::Font* m_font;
		bool		m_ready;
	};
}

