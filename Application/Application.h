#pragma once
#include <Base/ApplicationHandle.h>

class Application : public S2DE::Core::ApplicationHandle
{
public:
	Application();
	virtual ~Application() final;

	virtual bool LoadResources() final;
	virtual void OnRender()  final;
	virtual void OnUpdate(float DeltaTime)  final;
	virtual void OnWindowResize() final;
	virtual void OnStart() final;
	virtual void OnClose() final;
	virtual void InputEvents() final;
	virtual void OnLostDevice() final;
	virtual void OnResetDevice() final;
};


