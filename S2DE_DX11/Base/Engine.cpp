#include "Engine.h"

#include "Base/Other/SplashScreen.h"
#include "Base/ApplicationHandle.h"

#define S2DE_EXIT_PROCESS() exit(EXIT_SUCCESS);

namespace S2DE
{
	Engine* Engine::m_engine;
	ApplicationHandle* Engine::m_app_handle;
	GameWindow* Engine::m_window;
	GameTime Engine::m_time;
	bool Engine::m_isEditor;
	Renderer* Engine::m_render;

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

		m_render = new Renderer();

		if (!m_render->Create())
			return;

		//Load engine resources, read main config, etc
		sp->SetLoadState("Load engine resources...");
		
		if (!LoadEngineResources())
			return;

		//Application On start
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

	void Engine::OnLoop()
	{
		m_render->Render();
	}

	void Engine::Destroy()
	{
		Logger::Log("Destroying engine...");

		m_app_handle->OnClose();
		m_render->Destroy();

		S2DE_EXIT_PROCESS();
	}

	bool Engine::LoadEngineResources()
	{
		return true;
	}
}