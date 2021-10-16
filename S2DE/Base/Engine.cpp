#include "Engine.h"

#include "Base/Other/SplashScreen.h"
#include "Base/ApplicationHandle.h"
#include "Base/DebugTools/VisualConsole.h"
#include "Base/GameWindow.h"
#include "Base/InputManager.h"
#include "Base/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/ImGui_Window.h"

#define S2DE_EXIT_PROCESS() exit(EXIT_SUCCESS);

namespace S2DE
{
	Engine* Engine::m_engine;
	ApplicationHandle* Engine::m_app_handle;
	GameWindow* Engine::m_window;
	GameTime Engine::m_time;
	bool Engine::m_isEditor;
	Renderer* Engine::m_render;
	InputManager* Engine::m_input_m;
	VisualConsole* Engine::m_console;
	ResourceManager Engine::m_resource_manager;
	SceneManager* Engine::m_scene_manager;

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
		Delete(m_console);
	}

	void Engine::RunEngineInEditorMode(ApplicationHandle* app_handle)
	{
		//TODO 
		//Load from config project name
		RunEngine(app_handle, std::string(), true);
	}

	void Engine::RunEngineInGameMode(ApplicationHandle* app_handle, std::string pname)
	{
		RunEngine(app_handle, pname, false);
	}

	void Engine::RunEngine(ApplicationHandle* app_handle, std::string pname, bool isEditor)
	{
		m_isEditor = isEditor;

		SplashScreen* sp = new SplashScreen();
		S2DE_ASSERT(sp->ShowSplashScreen(GetModuleHandle(NULL)));

		m_console = new VisualConsole();
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
		Sleep(1000);

		m_window = new GameWindow();
		m_window->Create(GetModuleHandle(NULL), pname.c_str());

		m_input_m = new InputManager();
		if (!m_input_m->Initialize())
			return;

		m_render = new Renderer();
		if (!m_render->Create())
			return;
		
		m_scene_manager = new SceneManager();
		m_scene_manager->CreateNewScene();

		//Load engine resources, read main config, etc
		sp->SetLoadState("Load engine resources...");
		
		if (!LoadEngineResources())
			return;

		//Application On start
		Logger::Log("Application OnStart event");
		m_app_handle->OnStart();

		sp->SetLoadState("Load game resources...");
		if (!m_app_handle->LoadResources())
		{
			S2DE_FATAL_ERROR("Failed to load application resources");
			return;
		}


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
			m_console->TougleDraw();
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
#endif

		if (Engine::isEditor())
		{
			if (m_input_m->IsKeyPressed(KeyCode::KEY_F10))
			{
				m_render->GetImGui_Window("EditorToolStrip")->ToggleDraw();
				m_render->GetImGui_Window("EditorObjectInspector")->ToggleDraw();
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
		m_render->Destroy();
		m_scene_manager->Clear();
		Engine::GetResourceManager().ClearAll();

		S2DE_EXIT_PROCESS();
	}

	bool Engine::LoadEngineResources()
	{
		S2DE_ASSERT(m_resource_manager.LoadDefaultTexture());
		S2DE_ASSERT(m_resource_manager.Load<Shader>("Sprite"));

		return true;
	}
}