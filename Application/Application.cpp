#include "Application.h"
#include <Base\CoreMinimal.h>
#include <GameObjects\TestObject.h>
#include <Scene\SceneManager.h>

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
	CreateGameObject<TestObject>("testobject", "GameObject", 1, Vector3(0,0,0));
	CreateGameObject<TestObject>("testobject1", "GameObject", 1, Vector3(2,0,0));
	CreateGameObject<TestObject>("testobject2", "GameObject", 1, Vector3(-2,0,0));
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
