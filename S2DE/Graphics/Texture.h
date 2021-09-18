#pragma once
#include "Base/Main/Common.h"
#include "IO/IO_File.h"
#include "IO/IO_Disposible.h"

namespace S2DE
{
	class S2DE_API Texture : public IO_File, public IO_Disposible
	{
	public:
		Texture();
		~Texture();

	private:

	};
}
