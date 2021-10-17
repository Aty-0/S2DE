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

bool Application::LoadResources()
{
	CreateGameObject<Sprite>("testobject", "GameObject", 1, Vector3(0,0,0));
	CreateGameObject<Sprite>("testobject1", "GameObject", 1, Vector3(2,0,0));
	CreateGameObject<Sprite>("testobject2", "GameObject", 1, Vector3(-2,0,0));
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
