#pragma once
#include "Base/Main/Common.h"

namespace S2DE::Core
{
	class S2DE_API ApplicationHandle
	{
	public: 
		ApplicationHandle() = default;

		//Application custom functions
		virtual ~ApplicationHandle() {}
		virtual bool LoadResources() { return true; }

		virtual void OnRender() {}
		virtual void OnUpdate(float DeltaTime) {}
		virtual void OnStart() {}
		virtual void OnClose() {}
		virtual void OnLostDevice() {}
		virtual void OnResetDevice() {}
		virtual void OnWindowResize() {}

		virtual void InputEvents() {}
	};
}

