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
	Engine::GetGameWindow()->SetMouseVisible(true);

	Sprite* sprite_test_f = CreateGameObject<Sprite>("block1", "GameObject", 1, Vector3(0, 5, 1));
	sprite_test_f->SetScale(Vector3(3,3,3));
	
	Sprite* sprite_test_s = CreateGameObject<Sprite>("block2", "GameObject", 1, Vector3(0, 5, -10));
	sprite_test_s->SetScale(Vector3(3,3,3));
	sprite_test_s->SetColor(Math::Color<float>(1.0f, 0.0f, 0.0f, 1.0f));

	Sprite* sprite_alpha_test2 = CreateGameObject<Sprite>("leaf", "GameObject", 1, Vector3(0, 5, -6));
	sprite_alpha_test2->LoadTexture("leaf001");
	sprite_alpha_test2->Alpha = true;
	
	Sprite* sprite_alpha_test = CreateGameObject<Sprite>("alpha", "GameObject", 1, Vector3(0, 5, -5));
	sprite_alpha_test->LoadTexture("Player");
	sprite_alpha_test->SetAtlasSize(Vector2(36, 65));
	sprite_alpha_test->SetAtlasFramePosition(0, 0);
	
	CreateGameObject<NoTextureTestObject>("rnd_color", "GameObject", 1, Vector3(Vector3(0, 3.5, -7.5)));

	//Create 5 test objects
	//for(std::int32_t i = 0; i <= 5; i++)
	//	CreateGameObject<Sprite>("testobject", "GameObject", 1, Vector3(i * 2, 0, i));
}

bool Application::LoadResources()
{
	return true;
}


static std::int32_t c = 0;
static std::int32_t d = 0;
static std::int32_t f = 0;

void Application::OnUpdate(float DeltaTime)
{
	//for(std::int32_t i = 0; i < c; i++)
	//	if(GetObjectByName<Sprite>("input_obj" + std::to_string(i)) != nullptr)
	//		GetObjectByName<Sprite>("input_obj" + std::to_string(i))->SetRotation_Y(GetObjectByName<Sprite>("input_obj" + std::to_string(i))->GetRotation().y + 1 * DeltaTime);

}

void Application::OnRender()
{
	
}

void Application::InputEvents()
{
	Vector3 pos = GetObjectByName<Camera>(S2DE_MAIN_CAMERA_NAME)->GetPosition();

	if (Engine::GetInputManager()->IsKeyDown(KeyCode::KEY_T))
	{
		CreateGameObject<Sprite>("input_obj" + std::to_string(c), "GameObject", 1, Vector3(pos.x, pos.y, -c));
		GetObjectByName<Sprite>("input_obj" + std::to_string(c))->SetAtlasSize(Vector2(33, 60));
		GetObjectByName<Sprite>("input_obj" + std::to_string(c))->SetAtlasFramePosition(0, 0);
		GetObjectByName<Sprite>("input_obj" + std::to_string(c))->LoadTexture("Player");
		GetObjectByName<Sprite>("input_obj" + std::to_string(c))->Alpha = true;
		GetObjectByName<Sprite>("input_obj" + std::to_string(c))->SetScale(Vector3(2.0f, 2.0f, 1.0f));
		c++;

	}

	if (Engine::GetInputManager()->IsKeyDown(KeyCode::KEY_Y))
	{
		CreateGameObject<TestObject>("test_obj" + std::to_string(d), "GameObject", 1, Vector3(pos.x, pos.y, -d));
		GetObjectByName<TestObject>("test_obj" + std::to_string(d))->LoadTexture("Player2");
		GetObjectByName<TestObject>("test_obj" + std::to_string(d))->SetScale(Vector3(2.0f, 2.0f, 1.0f));
		d++;
	}


	if (Engine::GetInputManager()->IsKeyDown(KeyCode::KEY_U))
	{
		CreateGameObject<Sprite>("inputd_obj" + std::to_string(d), "GameObject", 1, Vector3(pos.x, pos.y, -d));
		GetObjectByName<Sprite>("inputd_obj" + std::to_string(d))->SetAtlasSize(Vector2(33, 60));
		GetObjectByName<Sprite>("inputd_obj" + std::to_string(d))->SetAtlasFramePosition(0, 0);
		GetObjectByName<Sprite>("inputd_obj" + std::to_string(d))->LoadTexture("Player2");
		GetObjectByName<Sprite>("inputd_obj" + std::to_string(d))->SetScale(Vector3(2.0f, 2.0f, 1.0f));
		d++;
	}

	if (Engine::GetInputManager()->IsKeyPressed(KeyCode::KEY_I))
	{
		CreateGameObject<TestObject>("finputd_obj" + std::to_string(f), "GameObject", 1, Vector3(pos.x, pos.y, -f));
		GetObjectByName<TestObject>("finputd_obj" + std::to_string(f))->SetScale(Vector3(2.0f, 2.0f, 1.0f));
		f++;
	}

	if (Engine::GetInputManager()->IsKeyDown(KeyCode::KEY_O))
	{
		CreateGameObject<NoTextureTestObject>("rnd_color", "GameObject", 1, Vector3(pos.x, pos.y, pos.z));
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

