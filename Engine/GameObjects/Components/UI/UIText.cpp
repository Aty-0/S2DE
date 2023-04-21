#define STB_TRUETYPE_IMPLEMENTATION 
#include "UIText.h"

#include "Base/Utils/Logger.h"

#include "Render/Font.h"
#include "GameObjects/Base/GameObject.h"
#include "GameObjects/Components/AlphaComponent.h"

#include "Base/ResourceManager.h"

#include "Render/Renderer.h"
#include "Render/Buffers.h"
#include "Render/Shader.h"


namespace S2DE::GameObjects::Components::UI
{
	UIText::UIText()
	{
		m_shader = new Render::Shader(*Core::Engine::GetResourceManager().Get<Render::Shader>("Text"));
	}

	UIText::~UIText()
	{
		m_vertexBuffers.clear();
		m_vertexBuffers.shrink_to_fit();
	}

	void UIText::OnCreate()
	{
		GetOwner()->CreateComponent<GameObjects::Components::AlphaComponent>();
	}

	void UIText::SetFont(Render::Font* font)
	{
		m_font = font;
	}

	void UIText::Rebuild()
	{
		if (m_font == nullptr)
		{
			m_ready = false;
			return;
		}

		if (m_text.empty())
		{
			m_ready = false;
			return;
		}

		// recreate buffer 
		for (auto vb : m_vertexBuffers)
		{
			Core::Delete(vb);
		}
		m_vertexBuffers.clear();
		m_vertexBuffers.shrink_to_fit();

		const char* text = m_text.c_str();

		float x = 0.0f, y = 0.0f; // text pos, not the gameobject 

		while (*text)
		{
			if (*text >= 32 && (std::uint8_t)(*text) < 128)
			{
				stbtt_aligned_quad q = { };
				stbtt_GetBakedQuad(m_font->GetBakedData(), m_font->GetTextureWidth(), m_font->GetTextureHeight(), 
					*text - 32, &x, &y, &q, 1);

				auto vertexBuffer = new Render::VertexBuffer<Render::Vertex>();


				if (*(text + 1))
				{
					stbtt_fontinfo info = m_font->GetFontInfo();

					float Scale = stbtt_ScaleForPixelHeight(&info, m_font->GetHeight());
					x += Scale * stbtt_GetCodepointKernAdvance(&info, *(text)-32, *(text + 1) - 32);
				}

				vertexBuffer->GetArray().push_back( { {   q.x1,  q.y0 , 0 }, {1, 1, 1, 1}, {   q.s1, q.t1 } });
				vertexBuffer->GetArray().push_back({ {    q.x0,  q.y0 , 0 }, {1, 1, 1, 1}, {   q.s0, q.t1 } });
				vertexBuffer->GetArray().push_back({ {    q.x1,  q.y1 , 0 },  {1, 1, 1, 1}, {  q.s1, q.t0 } });
				vertexBuffer->GetArray().push_back({ {    q.x0,  q.y1 , 0 },  {1, 1, 1, 1}, {  q.s0, q.t0 } } );


				Assert(vertexBuffer->Create(), "Failed to create vertex buffer");
				vertexBuffer->Update();

				m_vertexBuffers.push_back(vertexBuffer);
			}

			text++;
		}



		m_ready = true;
	}

	void UIText::SetText(const char* text, ...)
	{
		char buffer[2048];

		va_list args;
		va_start(args, text); 
		std::vsnprintf(buffer, sizeof(buffer), text, args); 
		va_end(args);	

		m_text = std::string(buffer);

		Rebuild();
	}

	void UIText::SetText(std::string text)
	{
		m_text = text;
		Rebuild();
	}

	void UIText::OnRender(Render::Renderer* renderer)
	{
		if (!m_ready)
		{
			return;
		}
		for (auto vb : m_vertexBuffers)
		{
			m_shader->UpdateMainConstBuffer(GetOwner()->GetTransform()->UpdateTransformation(), true);
			m_shader->Bind();

			m_font->GetFontTexture()->Bind();

			vb->Bind();
			renderer->Draw(vb->GetArray().size(), 0, D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			vb->Unbind();
			m_font->GetFontTexture()->Unbind();
		}
	}
}