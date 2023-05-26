#pragma once
#include "GameObjects/Components/UI/UIDrawableComponent.h"
#include "Math/Color.h"

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


		void SetText(const char* text, ...);
		void SetText(std::string text);

		void SetFont(std::string fontName);
		void SetFont(Render::Font* font);

		void SetColor(Math::Color<float> color);

		void SetHeight(float height);


		void OnCreate() override;
		void OnRender(Render::Renderer* renderer) override;

		[[nodiscard]] inline std::string   GetText() const;
		[[nodiscard]] inline Render::Font* GetFont() const;
		[[nodiscard]] inline Math::Color<float> GetColor() const;
	private:
		Render::Shader* m_shader;
		Render::VertexBuffer<Render::Vertex>* m_vertexBuffer;
		std::string   m_text;
		Render::Font* m_font;
		Math::Color<float> m_color;
		

		// Make text's quads
		bool Rebuild();
	};
}

