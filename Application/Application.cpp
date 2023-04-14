#include "Application.h"
#include <Base\CoreMinimal.h>

#include <Scene\SceneManager.h>

#include <GameObjects\Base\GameObject.h>
#include <GameObjects\Components\StaticMesh.h>
#include <GameObjects\Components\Light\PointLight.h>
#include <GameObjects\Components\Light\AmbientLight.h>
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
	auto camera = S2DE::Scene::GetObjectByName<GameObject>(S2DE_MAIN_CAMERA_NAME)->GetComponent<GameObjects::Components::Camera>();
	camera->SetProjectionMode(S2DE::GameObjects::Components::Camera::CameraProjectionMode::Perspective);

	Texture* texture = new Texture();
	texture->CreateEmptyTexture();
	Assert(Engine::GetResourceManager().Add(texture) == true, "");

	auto lightambientgo = CreateGameObject<GameObject>("lightambientgo", "GameObject", 1, Vector3(0, 0, 0));
	auto lightambientc = lightambientgo->CreateComponent<GameObjects::Components::Light::AmbientLight>();

	Math::Color<float> c = Math::Color<float>(0.15f, 0.1f, 0.2f, 1);
	lightambientc->SetColor(c);

	Math::Color<float> red = Math::Color<float>(1, 0, 0, 1);
	auto lightgo = CreateGameObject<GameObject>("PointLight", "GameObject", 1, Vector3(0, 0, 0));
	auto lightc = lightgo->CreateComponent<GameObjects::Components::Light::PointLight>();
	lightc->SetColor(red);
	lightgo->GetTransform()->SetParent(GetObjectByName<GameObject>(S2DE_MAIN_CAMERA_NAME));
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
		Engine::GetResourceManager().DumpAllResources();
	}

	if (Engine::GetInputManager()->IsKeyPressed(KeyCode::KEY_T))
	{
		auto lightgo = GetObjectByName<GameObject>("PointLight");
		if (lightgo != nullptr)
		{
			lightgo->GetTransform()->SetPosition(Vector3(5, 0, 0));
			auto lightc = lightgo->CreateComponent<GameObjects::Components::Light::PointLight>();
			Math::Color<float> blue = Math::Color<float>(1, 1, 1, 1);
			lightc->SetColor(blue);
		}

	}

	if (Engine::GetInputManager()->IsKeyPressed(KeyCode::KEY_O))
	{
		auto meshGO = CreateGameObject<GameObject>("Sponza", "GameObject", 1, Vector3(Vector3(0, 0, 0)));
		meshGO->GetTransform()->SetScale(DirectX::SimpleMath::Vector3(0.15f, 0.15f, 0.15f));
		auto meshC = meshGO->CreateComponent<Components::StaticMesh>();
		meshC->LoadMesh("Sponza");
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

