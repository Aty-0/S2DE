#include "Engine.h"

#include "Base/Other/SplashScreen.h"
#include "Base/ApplicationHandle.h"
#include "Base/GameWindow.h"
#include "Base/InputManager.h"
#include "Base/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/ImGui_Window.h"
#include "Editor/EditorCursor.h"

using namespace S2DE::Scene;
using namespace S2DE::Math;
using namespace S2DE::Render;
using namespace S2DE::Core::Other;

#define S2DE_EXIT_PROCESS() exit(EXIT_SUCCESS);

namespace S2DE::Core
{
	Engine* Engine::m_engine;
	ApplicationHandle* Engine::m_app_handle;
	GameWindow* Engine::m_window;
	GameTime Engine::m_time;
	bool Engine::m_isEditor;
	Renderer* Engine::m_render;
	InputManager* Engine::m_input_m;
	ResourceManager Engine::m_resource_manager;
	SceneManager* Engine::m_scene_manager;
	std::string Engine::m_params;

	Engine::Engine()
	{
		m_engine = this;
	}

	Engine::~Engine()
	{
		Delete(m_engine);
		Delete(m_app_handle);
		Delete(m_scene_manager);
		Delete(m_render);
		Delete(m_window);		
		Delete(m_input_m);
	}

	void Engine::RunEngineInEditorMode(ApplicationHandle* app_handle)
	{
		RunEngine(app_handle, std::string(), true);
	}

	void Engine::RunEngineInGameMode(ApplicationHandle* app_handle, std::string pname)
	{
		RunEngine(app_handle, pname, false);
	}

	void Engine::RunEngine(ApplicationHandle* app_handle, std::string pname, bool isEditor)
	{
		m_isEditor = isEditor;

		//Get params
		m_params = GetCommandLineA();

		SplashScreen* sp = new SplashScreen();
		S2DE_ASSERT(sp->ShowSplashScreen(GetModuleHandle(NULL)));

		//Set project name
		sp->SetProjectName(pname);
		//Create log file 
		Logger::CreateLogFile();

		Logger::Log("Starting engine...");
		sp->SetLoadState("Starting engine...");

		//Check application handle
		if ((m_app_handle = app_handle) == nullptr)
		{
			S2DE_FATAL_ERROR("No application handle setted");
			return;
		}

		//Delay for showing splash screen
		//Sleep(1000);

		sp->SetLoadState("Create game window...");
		m_window = new GameWindow();
		m_window->Create(GetModuleHandle(NULL), pname.c_str());

		sp->SetLoadState("Initialize input manager...");
		m_input_m = new InputManager();
		if (!m_input_m->Initialize())
			return;

		sp->SetLoadState("Initialize render...");
		m_render = new Renderer();
		if (!m_render->Create())
			return;
		
		sp->SetLoadState("Initialize scene...");
		m_scene_manager = new SceneManager();
		m_scene_manager->CreateNewScene();

		//Load engine resources, read main config, etc
		sp->SetLoadState("Load engine resources...");
		
		if (!LoadEngineResources())
			return;

		sp->SetLoadState("Load game resources...");
		if (!m_app_handle->LoadResources())
		{
			S2DE_FATAL_ERROR("Failed to load application resources");
			return;
		}

		//Application On start
		Logger::Log("Application OnStart event");
		m_app_handle->OnStart();


		//Close and destroy splash screen
		sp->Close();
		Delete(sp);
		   
		//Run main game loop
		RunLoop();

	}

	void Engine::RunLoop()
	{
		while (m_window->ProcessMessage() == true && m_window->isClosing() == false)
		{
			OnLoop();
		}
	}

	void Engine::UpdateEngineInputKeys()
	{
		if (m_input_m->IsKeyPressed(KeyCode::KEY_ESCAPE))
		{
			m_engine->Destroy();
		}

		if (m_input_m->IsKeyPressed(KeyCode::KEY_GRAVE))
		{
			m_render->GetImGui_Window("EngineConsole")->ToggleDraw();
		}

		if (m_input_m->IsKeyPressed(KeyCode::KEY_F11))
		{
			m_window->SetFullscreen(!m_window->isFullscreen());
		}

#ifdef _DEBUG
		if (m_input_m->IsKeyPressed(KeyCode::KEY_0))
		{
			m_render->GetImGui_Window("DebugInfoWindow")->ToggleDraw();
		}

		if (m_input_m->IsKeyPressed(KeyCode::KEY_9))
		{
			m_render->GetImGui_Window("DebugObjectInspectorWindow")->ToggleDraw();
		}

#ifdef S2DE_DEBUG_RENDER_MODE
		if (Engine::isEditor())
		{
			if (m_input_m->IsKeyPressed(KeyCode::KEY_F9))
			{
				m_render->ToggleImGuiDemoWindowVisible();
			}
		}
#endif


#endif
		if (Engine::isEditor())
		{
			if (m_input_m->IsKeyPressed(KeyCode::KEY_F1))
			{
				m_render->ToggleImGuiWindowsVisible();
			}
		}

	}

	void Engine::UpdateInput()
	{
		//Update input manager
		if (m_input_m->Update())
		{
			UpdateEngineInputKeys();
			m_app_handle->InputEvents();
			m_scene_manager->UpdateInput();
		}
	}

	void Engine::OnGlobalUpdate(float DeltaTime)
	{
		m_scene_manager->UpdateScene(DeltaTime);
		m_app_handle->OnUpdate(DeltaTime);

		if (isEditor())
			m_render->GetEditorCursor()->Update(DeltaTime);
	}

	void Engine::OnLoop()
	{
		UpdateInput();
		m_time.Tick();

		OnGlobalUpdate(m_time.GetDeltaTime());
		m_render->Render();
	}

	void Engine::Destroy()
	{
		Logger::Log("Destroying engine...");

		m_app_handle->OnClose();
		m_scene_manager->Clear();
		Engine::GetResourceManager().ClearAll();
		m_render->Destroy();

		S2DE_EXIT_PROCESS();
	}

	bool Engine::LoadEngineResources()
	{
		S2DE_ASSERT(m_resource_manager.LoadDefaultTexture());
		S2DE_ASSERT(m_resource_manager.Load<Shader>("Sprite"));
		S2DE_ASSERT(m_resource_manager.Load<Shader>("Line"));

		return true;
	}
}