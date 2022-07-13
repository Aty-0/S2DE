#include "Application.h"
#include <Base\CoreMinimal.h>
#include <GameObjects\NoTextureTestObject.h>
#include <GameObjects\TestObject.h>


using namespace S2DE;
using namespace S2DE::GameObjects;
using namespace S2DE::Scene;
using namespace S2DE::Math;
using namespace S2DE::Render;
using namespace S2DE::Core;
using namespace S2DE::Core::Other;
using namespace DirectX::SimpleMath;



Application::Application()
{
	Engine* engine = new Engine();


	engine->RunEngineInEditorMode(this);
	//engine->RunEngineInGameMode(this);
}

Application::~Application()
{

}


void Application::OnStart()
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

