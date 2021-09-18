#include "Application.h"
#include <Base\CoreMinimal.h>
#include <GameObjects\TestObject.h>


Application::Application()
{
	Engine* engine = new Engine();


	engine->RunGame(this);
}

Application::~Application()
{

}

TestObject* t;
bool Application::LoadResources()
{
	t = new TestObject();
	return true;
}


void Application::OnUpdate(float DeltaTime)
{

}

void Application::OnRender()
{
	t->Render();
}

void Application::InputEvents()
{

}

void Application::OnResetDevice()
{

}

void Application::OnLostDevice()
{

}

void Application::OnClose()
{

}

void Application::OnWindowResize()
{

}


void Application::OnStart()
{

}
