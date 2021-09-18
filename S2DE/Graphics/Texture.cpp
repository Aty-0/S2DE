#include "Texture.h"

namespace S2DE
{
	Texture::Texture() 
	{
		m_type = "Texture";
		m_ex = { ".dds", ".png", ".tga", ".jpg" };
	}

	Texture::~Texture()
	{

	}
}