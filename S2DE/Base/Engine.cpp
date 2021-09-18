#include "Engine.h"

#include "Base/Other/SplashScreen.h"
#include "Base/ApplicationHandle.h"
#include "Base/DebugTools/VisualConsole.h"
#include "Base/GameWindow.h"
#include "Base/InputManager.h"
#include "Base/ResourceManager.h"
#include "Graphics/Renderer.h"

#include "GameObjects/Camera.h"


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

	Engine::Engine()
	{
		m_engine = this;
	}

	Engine::~Engine()
	{
		Delete(m_engine);
		Delete(m_app_handle);
		Delete(m_render);
		Delete(m_window);		
	}

	void Engine::RunGame(ApplicationHandle* app_handle, std::string pname)
	{
		RunEngine(app_handle, pname, false);
	}

	void Engine::RunEngine(ApplicationHandle* app_handle, std::string pname, bool isEditor)
	{
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
		Camera::_Camera->UpdateControl();
		Camera::_Camera->UpdateMouseControl();

		if (m_input_m->IsKeyPressed(KeyCode::KEY_GRAVE))
		{
			m_console->TougleDraw();
		}

		if (m_input_m->IsKeyPressed(KeyCode::KEY_ESCAPE))
		{
			m_engine->Destroy();
		}
	}

	void Engine::UpdateInput()
	{
		//Update input manager
		if (m_input_m->Update())
		{
			//Get input events from application
			m_app_handle->InputEvents();
			//Get in engine key events
			UpdateEngineInputKeys();
		}
	}

	void Engine::OnGlobalUpdate(float DeltaTime)
	{
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
		Engine::GetResourceManager().ClearAll();

		S2DE_EXIT_PROCESS();
	}

	bool Engine::LoadEngineResources()
	{
		S2DE_ASSERT(m_resource_manager.LoadDefaultTexture());
		Engine::GetResourceManager().Load<Shader>("test");

		return true;
	}
}