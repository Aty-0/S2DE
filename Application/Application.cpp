#include "Application.h"
#include <Base\CoreMinimal.h>


Application::Application()
{
	Engine* engine = new Engine();


	engine->RunEngineInEditorMode(this);
}

Application::~Application()
{

}

void Application::OnStart()
{
	//Create 5 test objects
	for(std::int32_t i = 0; i <= 5; i++)
		CreateGameObject<Sprite>("testobject", "GameObject", 1, Vector3(i * 2, 0, 0));
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

