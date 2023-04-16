#include "Font.h"

#include "Base/Utils/Logger.h"
#include "Render/Texture.h"

#include <fstream>


namespace S2DE::Render
{
	Font::Font()
	{
		m_type = "Font";
		m_extensions = { ".ttf" };
	}

	Font::~Font()
	{

	}

	bool Font::Load(std::string name)
	{
		const auto paths = FindPath({ name });
		if (m_notLoaded == true)
		{
			return false;
		}
		// Get font path from list  
		const auto path = paths[0];

		auto file = std::ifstream(path, std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<char> buffer(size);

		if (!file.read(buffer.data(), size))
		{
			return false;
		}

		file.close();

		// use stbtt lib for using our font data
		// so, we are initialize font in this step...
		stbtt_fontinfo info = { };		
		Verify(stbtt_InitFont(&info, reinterpret_cast<std::uint8_t*>(buffer.data()), 0) == 1, "Can't initialize font!");

		// TODO: Make it customizable!
		const std::int16_t b_w = 512; // bitmap width 
		const std::int16_t b_h = 512; // bitmap height 
		const float pixel_h = 32;  // line height 

		// create a bitmap for the phrase 
		std::uint8_t* bitmap = reinterpret_cast<std::uint8_t*>(calloc(b_w * b_h, sizeof(std::uint8_t)));
		
		stbtt_BakeFontBitmap(reinterpret_cast<std::uint8_t*>(buffer.data()), 0, pixel_h, bitmap, b_w, b_h, 32, 96, m_cdata); 


		// push data to texture 
		m_fontTexture = new Texture();
		Verify(m_fontTexture->CreateFontTexture(b_w, b_h, bitmap), "Font texture is not created!");


		delete[] bitmap;
		buffer.clear();
		buffer.shrink_to_fit();

		return true;
	}

	[[nodiscard]] inline Texture* Font::GetFontTexture() const
	{
		return m_fontTexture;
	}

}