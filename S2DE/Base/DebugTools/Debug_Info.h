#pragma once
#include "Base/Main/Common.h"
#include "Base/DebugTools/Debug_Window.h"

namespace S2DE
{
	class S2DE_API Debug_Info : public Debug_Window
	{
	public:
		Debug_Info();
		~Debug_Info();

		virtual void Render() final;

	};

}