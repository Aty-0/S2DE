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
		Core::Delete(m_fontTexture);
		Core::Delete(m_ttf_buffer);
		if (m_cdata)
		{
			delete[] m_cdata;
		}
	}

	bool Font::Create(float height, std::uint16_t w, std::uint16_t h)
	{
		m_height = height;

		m_tex_w = w;
		m_tex_h = h;

		// make font bitmap 
		std::uint8_t* bitmap = new uint8_t[w * h];
		stbtt_BakeFontBitmap(m_ttf_buffer, 0, height, bitmap, m_tex_w, m_tex_h, 32, 96, m_cdata);

		// push data to texture 
		m_fontTexture = new Texture();
		Verify(m_fontTexture->CreateFontTexture(w, h, bitmap), "Font texture is not created!");
		delete[] bitmap;

		return Core::Resources::ResourceManager::GetInstance()->Add(m_fontTexture, m_name + "_fnt_tex", m_cantDelete);
	}

	bool Font::Load(std::string name)
	{
		// Try to found our file
		const auto paths = FindPath({ name });
		Verify(!m_notLoaded, "File is not found or has different format, abort!");
	
		// Get font path from list  
		const auto path = paths[0];

		// Create ttf buffer...
		// Open out ttf file 
		auto file = std::ifstream(path, std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		// All file data we are save to the ttf buffer
		// So, we can use it for diffrent purposes 
		m_ttf_buffer = reinterpret_cast<std::uint8_t*>(malloc(size));
		Verify(file.read(reinterpret_cast<char*>(m_ttf_buffer), size), "Can't create ttf buffer!");
		file.close();

		// Try to initialize font by parse our buffer to stbtt
		Verify(stbtt_InitFont(&m_info, m_ttf_buffer, 0) == 1, "Can't initialize font!");

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

	[[nodiscard]] inline std::uint8_t* Font::GetTTFData() 
	{
		return m_ttf_buffer;
	}

	[[nodiscard]] inline Texture* Font::GetFontTexture() const
	{
		return m_fontTexture;
	}

}