#pragma once
#include "Base/Main/Common.h"
#include "Render/ImGui_Window.h"

namespace S2DE::Core::Debug
{
	class S2DE_API Debug_Info : public Render::ImGui_Window
	{
	public:
		Debug_Info();
		~Debug_Info();

		virtual void Render() final;

	};

}