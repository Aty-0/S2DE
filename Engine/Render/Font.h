#pragma once
#include "Base/Main/Common.h"
#include "Base/Resource.h"

#define STB_TRUETYPE_IMPLEMENTATION 
#include "Libs/stb_truetype.h"

namespace S2DE::Render
{
	class Texture;
	class S2DE_API Font : public Core::Resources::Resource
	{
	public:
		Font();
		~Font();


		bool Load(std::string name) final;
		[[nodiscard]] inline Texture* GetFontTexture() const;

	private:
		Texture* m_fontTexture;
		stbtt_bakedchar m_cdata[96];
	};
}