#pragma once
#include <Base/ApplicationHandle.h>

namespace S2DE::Render
{
	class Renderer;
}

class Application : public S2DE::Core::ApplicationHandle
{
public:
	Application();
	virtual ~Application() final;

	virtual bool LoadResources() final;
	virtual void OnRender(S2DE::Render::Renderer* renderer)  final;
	virtual void OnUpdate(float deltaTime)  final;
	virtual void OnWindowResize() final;
	virtual void OnStart() final;
	virtual void OnClose() final;
	virtual void InputEvents() final;
	virtual void OnLostDevice() final;
	virtual void OnResetDevice() final;
};


