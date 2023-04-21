#include "Font.h"

#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Base/Utils/Logger.h"
#include "Render/Texture.h"

#include <fstream>


namespace S2DE::Render
{
	Font::Font() : 
		m_info(),
		m_fontTexture(nullptr),
		m_cdata()
	{
		m_type = "Font";
		m_extensions = { ".ttf" };
	}

	Font::~Font()
	{
		m_ttf_buffer.clear();
		m_ttf_buffer.shrink_to_fit();

		Core::Delete(m_fontTexture);
		delete[] m_cdata;
	}

	bool Font::Create(float height, std::uint16_t w, std::uint16_t h)
	{
		m_height = height;

		m_tex_w = w;
		m_tex_h = h;

		// make font bitmap 
		std::uint8_t* bitmap = new uint8_t[w * h];
		// 96
		stbtt_BakeFontBitmap(reinterpret_cast<std::uint8_t*>(m_ttf_buffer.data()), 0, height, bitmap, m_tex_w, m_tex_h, 32, 96, m_cdata);

		// push data to texture 
		m_fontTexture = new Texture();
		Verify(m_fontTexture->CreateFontTexture(w, h, bitmap), "Font texture is not created!");
		delete[] bitmap;

		return Core::Engine::GetResourceManager().Add(m_fontTexture, m_name + "_fnt_tex", m_cantDelete);
	}

	bool Font::Load(std::string name)
	{
		// Try to found our file
		const auto paths = FindPath({ name });
		Verify(!m_notLoaded, "File is not found or has different format, abort!");
	
		// Get font path from list  
		const auto path = paths[0];

		// Create ttf buffer...
		auto file = std::ifstream(path, std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		m_ttf_buffer = std::vector<char>(size);
		Verify(file.read(m_ttf_buffer.data(), size), "Can't create ttf buffer!");
		file.close();

		// use stbtt lib for using our font data
		// so, we are initialize font in this step...
		Verify(stbtt_InitFont(&m_info, reinterpret_cast<std::uint8_t*>(m_ttf_buffer.data()), 0) == 1, "Can't initialize font!");

		return true;
	}

	[[nodiscard]] inline std::uint16_t  Font::GetTextureWidth()
	{
		return m_tex_w;
	}

	[[nodiscard]] inline std::uint16_t  Font::GetTextureHeight()
	{
		return m_tex_h;
	}

	[[nodiscard]] inline float Font::GetHeight()
	{
		return m_height;
	}

	[[nodiscard]] inline stbtt_fontinfo Font::GetFontInfo()
	{
		return m_info;
	}

	[[nodiscard]] inline stbtt_bakedchar* Font::GetBakedData()
	{
		return m_cdata;
	}

	[[nodiscard]] inline Texture* Font::GetFontTexture() const
	{
		return m_fontTexture;
	}

}