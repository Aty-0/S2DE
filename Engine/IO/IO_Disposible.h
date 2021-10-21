#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/Logger.h"

namespace S2DE
{
	class S2DE_API IO_Disposible
	{
	public:
		virtual void Cleanup() { Logger::Error("Cleanup function not overridden"); }
	};
}