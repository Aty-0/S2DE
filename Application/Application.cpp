#include "Application.h"
#include <Base\CoreMinimal.h>
#include <GameObjects\Components\StaticMesh.h>

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
		auto meshGO = CreateGameObject<GameObject>("Sponza", "GameObject", 1, Vector3(Vector3(0, 0, 0)));
		meshGO->GetTransform()->SetScale(DirectX::SimpleMath::Vector3(0.15f, 0.15f, 0.15f));
		auto meshC = meshGO->CreateComponent<Components::StaticMesh>();
		meshC->LoadMesh("Sponza");
		auto testGet = meshGO->GetComponent<Components::StaticMesh>();
		if (testGet != nullptr)
			Logger::Log("%s %s %s", testGet->GetOwner()->GetName().c_str(), testGet->GetName().c_str(), testGet->GetUUIDString().c_str());
		else
			Logger::Error("get error");
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

