#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/Logger.h"

namespace S2DE::IO
{
	class S2DE_API IO_Disposible
	{
	public:
		virtual void Cleanup() { Core::Utils::Logger::Error("Cleanup function not overridden"); }
	};
}