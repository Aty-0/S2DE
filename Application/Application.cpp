#include "Application.h"
#include <Base\CoreMinimal.h>
#include <Math\Normalize.h>
#include <GameObjects\NoTextureTestObject.h>
#include <GameObjects\TestObject.h>
#include <GameObjects\StaticMesh.h>

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
	Logger::Log("%f", Normalize(2.0f));
	Engine::GetGameWindow()->SetMouseVisible(true);
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
	if (Engine::GetInputManager()->IsKeyDown(KeyCode::KEY_O))
	{
		StaticMesh* mesh = CreateGameObject<StaticMesh>("Sponza", "GameObject", 1, Vector3(Vector3(0, 0, 0)));
		mesh->LoadMesh("Sponza");
		mesh->SetScale(DirectX::SimpleMath::Vector3(0.15f, 0.15f, 0.15f));
	}
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

