#pragma once
#include "Base/Main/Common.h"
#include "Base/Resource.h"

#include "Libs/stb_truetype.h"

namespace S2DE::Render
{
	class Texture;
	class S2DE_API Font : public Core::Resources::Resource
	{
	public:
		Font();
		~Font();

		bool Create(float height = 32.0f, std::uint16_t w = 512, std::uint16_t h = 512);
		bool Load(std::string name);

		[[nodiscard]] inline Texture* GetFontTexture() const;
		[[nodiscard]] inline stbtt_bakedchar* GetBakedData();
		[[nodiscard]] inline stbtt_fontinfo   GetFontInfo();
		[[nodiscard]] inline float   GetHeight();

		[[nodiscard]] inline std::uint16_t GetTextureWidth();
		[[nodiscard]] inline std::uint16_t GetTextureHeight();

	private:
		Texture*		m_fontTexture;
		stbtt_bakedchar m_cdata[96];
		stbtt_fontinfo	m_info;
		
		std::vector<char> m_ttf_buffer;
		float			  m_height;
		std::uint16_t	  m_tex_w;
		std::uint16_t	  m_tex_h;
	};
}