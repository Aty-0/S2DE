#pragma once
#include "Base/Main/Common.h"

#include "Graphics/ImGui_Window.h"

namespace S2DE::Core::Debug
{
	//TODO
	//Keyboard manipulate
	//Draw parameters by object type
	//Set parameters
	//Better design

	class S2DE_API Debug_ObjectInspector : public Render::ImGui_Window
	{
	public:
		Debug_ObjectInspector();
		~Debug_ObjectInspector();

		virtual void Render() final;

	private:
		std::string m_sl_item_name = std::string();

	};

}