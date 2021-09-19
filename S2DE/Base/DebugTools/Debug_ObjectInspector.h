#pragma once
#include "Base/Main/Common.h"

#include "Base/DebugTools/Debug_Window.h"

namespace S2DE
{
	//TODO
	//Keyboard manipulate
	//Draw parameters by object type
	//Set parameters
	//Better design

	class S2DE_API Debug_ObjectInspector : public Debug_Window
	{
	public:
		Debug_ObjectInspector();
		~Debug_ObjectInspector();

		virtual void Render() final;

	private:
		std::string m_sl_item_name = std::string();

	};

}