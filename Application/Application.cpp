#include "Application.h"

#include <Base\Engine.h>

Application::Application()
{
	Engine* engine = new Engine();


	engine->RunGame(this);
}


Application::~Application()
{

}


bool Application::LoadResources()
{
	return true;
}


void Application::OnUpdate(float DeltaTime)
{

}

void Application::OnRender()
{

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
