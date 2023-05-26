#define STB_TRUETYPE_IMPLEMENTATION 
#include "UIText.h"

#include "Base/Utils/Logger.h"
#include "Base/ResourceManager.h"

#include "GameObjects/Base/GameObject.h"
#include "GameObjects/Components/AlphaComponent.h"

#include "Render/Font.h"
#include "Render/Renderer.h"
#include "Render/Texture.h"
#include "Render/Buffers.h"
#include "Render/Shader.h"


namespace S2DE::GameObjects::Components::UI
{
	UIText::UIText() :
		m_color({255,255,255,255}),
		m_font(nullptr),
		m_shader(new Render::Shader(*Core::Resources::ResourceManager::GetInstance()->Get<Render::Shader>("Text"))),
		m_text(),
		m_vertexBuffer()
	{
	}

	UIText::~UIText()
	{
		Core::Delete(m_vertexBuffer);
		Core::Delete(m_font); // Delete our font copy 
	}

	void UIText::OnCreate()
	{
		const auto alphaComp = GetOwner()->CreateComponent<GameObjects::Components::AlphaComponent>();
		alphaComp->SetAlpha(true);
	}

	void UIText::SetHeight(float height)
	{
		// Recreate font...
		m_font->Create(height);
	}

	void UIText::SetColor(Math::Color<float> color)
	{
		m_color = color;
	}

	void UIText::SetFont(std::string nameFont)
	{
		const auto resourceManager = Core::Resources::ResourceManager::GetInstance();

		auto font = resourceManager->Get<Render::Font>(nameFont);
		if (font == nullptr)
		{
			bool result = resourceManager->Load<Render::Font>(nameFont);
			if (result)
			{
				Core::Utils::Logger::Error("Font is not found in resources!");
				return;
			}

			font = resourceManager->Get<Render::Font>(nameFont);
		}

		m_font = new Render::Font(*font);
	}

	void UIText::SetFont(Render::Font* font)
	{
		if (font == nullptr)
		{
			Core::Utils::Logger::Error("Font is null...");
			return;
		}

		m_font = new Render::Font(*font);
	}

	bool UIText::Rebuild()
	{
		if (m_font == nullptr)
		{
			return false;
		}

		if (m_text.empty())
		{
			return false;
		}

		m_vertexBuffer = new Render::VertexBuffer<Render::Vertex>();
		
		const char* text = m_text.c_str();
		
		float x = 0;
		float y = 0;
		float y1 = 0;

		auto info = m_font->GetFontInfo();
		const float scale = stbtt_ScaleForPixelHeight(&info, m_font->GetHeight()) * 4;
		while (*text)
		{
			if (static_cast<std::uint8_t>(*text) >= 32 && static_cast<std::uint8_t>(*text) < 128)
			{
				stbtt_bakedchar*   bc = m_font->GetBakedData();
				stbtt_aligned_quad q = { };
				stbtt_GetBakedQuad(bc, m_font->GetTextureWidth(), m_font->GetTextureHeight(), 
					*text - 32, &x, &y, &q, 0);
			
				float x1 = q.x0 * scale;
				float x2 = q.x1 * scale;
				float y2 = -q.y0 * scale;
				float _y1 = q.y1 * scale;
				y1 = std::min(y1, _y1);
						
				m_vertexBuffer->GetArray().push_back({ { x1, y2, 0 },  { m_color.r, m_color.g, m_color.b, m_color.a }, {  q.s0, q.t0 } });
				m_vertexBuffer->GetArray().push_back({ { x2, y2, 0 },  { m_color.r, m_color.g, m_color.b, m_color.a }, {  q.s1, q.t0 } });
				m_vertexBuffer->GetArray().push_back({ { x1, y1, 0 },  { m_color.r, m_color.g, m_color.b, m_color.a }, {  q.s0, q.t1 } });
				m_vertexBuffer->GetArray().push_back({ { x2, y1, 0 },  { m_color.r, m_color.g, m_color.b, m_color.a }, {  q.s1, q.t1 } });

			}

			text++;
		}


		Assert(m_vertexBuffer->Create(), "Failed to create vertex buffer");
		return true;
	}

	void UIText::SetText(const char* text, ...)
	{
		char buffer[2048];

		va_list args;
		va_start(args, text); 
		std::vsnprintf(buffer, sizeof(buffer), text, args); 
		va_end(args);	

		m_text = std::string(buffer);
	}

	void UIText::SetText(std::string text)
	{
		m_text = text;
	}

	void UIText::OnRender(Render::Renderer* renderer)
	{
		if (!Rebuild())
			return;

		m_shader->UpdateMainConstBuffer(renderer, GetOwner()->GetTransform()->UpdateTransformation2D(), true);
		m_shader->Bind(renderer);

		m_font->GetFontTexture()->Bind(renderer);

		m_vertexBuffer->Bind(renderer);
		renderer->Draw(m_vertexBuffer->GetArray().size(), 0, D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		m_vertexBuffer->Unbind(renderer);
		m_font->GetFontTexture()->Unbind(renderer);

		Core::Delete(m_vertexBuffer);
	}

	[[nodiscard]] inline std::string UIText::GetText() const
	{
		return m_text;
	}

	
	[[nodiscard]] inline Render::Font* UIText::GetFont() const
	{
		return m_font;
	}

	[[nodiscard]] inline Math::Color<float>  UIText::GetColor() const
	{
		return m_color;
	}
}