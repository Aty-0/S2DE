#include "Application.h"
#include <Base\CoreMinimal.h>
#include <GameObjects\TestObject.h>


using namespace S2DE;
using namespace S2DE::GameObjects;
using namespace S2DE::Scene;
using namespace S2DE::Math;
using namespace S2DE::Render;
using namespace S2DE::Core;
using namespace S2DE::Core::Other;
using namespace S2DE::Core::Debug;


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
		CreateGameObject<Sprite>("testobject", "GameObject", 1, Vector3(i * 2, 0, i));

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
	if (Engine::GetInputManager()->IsKeyDown(KeyCode::KEY_Y))
		Engine::GetSceneManager()->GetScene()->Delete("input_obj");

	if (Engine::GetInputManager()->IsKeyDown(KeyCode::KEY_T))
	{
		Vector3 pos = GetObjectByName<Camera>(S2DE_MAIN_CAMERA_NAME)->GetPosition();
		static std::int32_t c = 0;
		c++;
		CreateGameObject<Sprite>("input_obj" + std::to_string(c), "GameObject", 1, Vector3(pos.x, pos.y, c));
		GetObjectByName<Sprite>("input_obj" + std::to_string(c))->SetAtlasSize(Vector2(33, 60));
		GetObjectByName<Sprite>("input_obj" + std::to_string(c))->SetAtlasFramePosition(0, 0);
		GetObjectByName<Sprite>("input_obj" + std::to_string(c))->LoadTexture("Player");
		GetObjectByName<Sprite>("input_obj" + std::to_string(c))->SetScale(Vector2(2.0f, 2.0f));

	}


	if (Engine::GetInputManager()->IsKeyDown(KeyCode::KEY_U))
	{
		Vector3 pos = GetObjectByName<Camera>(S2DE_MAIN_CAMERA_NAME)->GetPosition();
		static std::int32_t d = 0;
		d++;
		CreateGameObject<Sprite>("inputd_obj" + std::to_string(d), "GameObject", 1, Vector3(pos.x, pos.y, d));
		GetObjectByName<Sprite>("inputd_obj" + std::to_string(d))->SetAtlasSize(Vector2(33, 60));
		GetObjectByName<Sprite>("inputd_obj" + std::to_string(d))->SetAtlasFramePosition(0, 0);
		GetObjectByName<Sprite>("inputd_obj" + std::to_string(d))->LoadTexture("Player2");
		GetObjectByName<Sprite>("inputd_obj" + std::to_string(d))->SetScale(Vector2(2.0f, 2.0f));

	}

	if (Engine::GetInputManager()->IsKeyPressed(KeyCode::KEY_P))
	{
		Vector3 pos = GetObjectByName<Camera>(S2DE_MAIN_CAMERA_NAME)->GetPosition();
		static std::int32_t c = 0;
		c++;
		CreateGameObject<Sprite>("input_obj" + std::to_string(c), "GameObject", 1, Vector3(pos.x, pos.y, c));
		GetObjectByName<Sprite>("input_obj" + std::to_string(c))->SetAtlasSize(Vector2(33, 60));
		GetObjectByName<Sprite>("input_obj" + std::to_string(c))->SetAtlasFramePosition(0, 0);
		GetObjectByName<Sprite>("input_obj" + std::to_string(c))->LoadTexture("Player");
		GetObjectByName<Sprite>("input_obj" + std::to_string(c))->SetScale(Vector2(2.0f, 2.0f));

	}


	if (Engine::GetInputManager()->IsKeyPressed(KeyCode::KEY_O))
	{
		Vector3 pos = GetObjectByName<Camera>(S2DE_MAIN_CAMERA_NAME)->GetPosition();
		static std::int32_t d = 0;
		d++;
		CreateGameObject<Sprite>("inputd_obj" + std::to_string(d), "GameObject", 1, Vector3(pos.x, pos.y, d));
		GetObjectByName<Sprite>("inputd_obj" + std::to_string(d))->SetAtlasSize(Vector2(33, 60));
		GetObjectByName<Sprite>("inputd_obj" + std::to_string(d))->SetAtlasFramePosition(0, 0);
		GetObjectByName<Sprite>("inputd_obj" + std::to_string(d))->LoadTexture("Player2");
		GetObjectByName<Sprite>("inputd_obj" + std::to_string(d))->SetScale(Vector2(2.0f, 2.0f));

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

