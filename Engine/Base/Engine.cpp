#include "Engine.h"

#include "Base/Other/SplashScreen.h"
#include "Base/ApplicationHandle.h"
#include "Base/GameWindow.h"
#include "Base/InputManager.h"
#include "Base/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Render/Renderer.h"
#include "Render/ImGui_Window.h"

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
	Resources::ResourceManager Engine::m_resource_manager;
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

		// Get params
		m_params = GetCommandLineA();

		bool splash = !CheckAppParam("-nsplash");

		SplashScreen* sp = nullptr;
		SplashScreen::SetProjectName(pname);

		if (splash)
		{
			sp = new SplashScreen();
			Assert(sp->ShowSplashScreen(GetModuleHandle(NULL)), "");
		}

		// Create log file 
		Logger::CreateLogFile();

		Logger::Log("Starting engine...");
		SplashScreen::SetLoadState("Starting engine...", sp);

		// Check application handle
		if ((m_app_handle = app_handle) == nullptr)
		{
			S2DE_FATAL_ERROR("No application handle setted");
			return;
		}

		SplashScreen::SetLoadState("Create game window...", sp);

		m_window = new GameWindow();
		m_window->Create(pname.c_str());
		if (splash)
			m_window->Hide();

		m_input_m = new InputManager();
		SplashScreen::SetLoadState("Initialize render...", sp);

		m_render = new Renderer();
		if (!m_render->Create())
			return;
		
		// Load engine resources, read main config, etc
		SplashScreen::SetLoadState("Load engine resources...", sp);
		
		if (!LoadEngineResources())
		{
			S2DE_FATAL_ERROR("Failed to load engine resources!");
			return;
		}

		SplashScreen::SetLoadState("Initialize scene...", sp);
		m_scene_manager = new SceneManager();
		m_scene_manager->CreateNewScene();

		SplashScreen::SetLoadState("Load game resources...", sp);
		if (!m_app_handle->LoadResources())
		{
			S2DE_FATAL_ERROR("Failed to load application resources!");
			return;
		}

		// Application On start
		Logger::Log("Application OnStart event");
		m_app_handle->OnStart();


		// Destroy splash screen and show game window
		if (splash)
		{
			sp->Close();
			Delete(sp);
		}
		m_window->Restore();

		// Run main game loop
		RunLoop();

	}

	void Engine::RunLoop()
	{
		while (m_window->PoolEvents())
			OnLoop();	
	}

	void Engine::UpdateEngineInputKeys()
	{
		if (m_input_m->IsKeyPressed(KeyCode::KEY_ESCAPE))
		{
			m_engine->Destroy();
		}

		if (m_input_m->IsKeyPressed(KeyCode::KEY_GRAVE))
		{
			m_render->GetImGui_Window<Render::ImGui_Window*>("EngineConsole")->ToggleDraw();
		}

		if (m_input_m->IsKeyPressed(KeyCode::KEY_F11))
		{
			m_window->SetFullscreen(!m_window->isFullscreen());
		}

		if (m_input_m->IsKeyPressed(KeyCode::KEY_0))
		{
			m_render->GetImGui_Window<Render::ImGui_Window*>("DebugInfoWindow")->ToggleDraw();
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
		m_app_handle->InputEvents();
		UpdateEngineInputKeys();
	}

	void Engine::OnGlobalUpdate(float DeltaTime)
	{
		m_scene_manager->UpdateScene(DeltaTime);
		m_app_handle->OnUpdate(DeltaTime);
	}

	void Engine::OnLoop()
	{
		m_time.Begin();
		UpdateInput();
		OnGlobalUpdate(m_time.GetDeltaTime());
		m_render->Render();
		m_time.End();
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
		// Load default texture
		Verify(m_resource_manager.Load<Render::Texture>("Engine/DefaultTexture", "DefaultTexture"), "Can't load default texture");
		Verify(m_resource_manager.Load<Render::Texture>("Engine/DefaultSky", "DefaultSky"), "Can't load default sky texture");
		// Load shaders 
		Verify(m_resource_manager.Load<Render::Shader>("Sprite"), "Can't load sprite shader");
		Verify(m_resource_manager.Load<Render::Shader>("Line"), "Can't load line shader");
		Verify(m_resource_manager.Load<Render::Shader>("Mesh"), "Can't load mesh shader");
		Verify(m_resource_manager.Load<Render::Shader>("Skybox"), "Can't load skybox shader");

		// Load editor resources 
		if (Engine::isEditor())
		{
			Verify(m_resource_manager.Load<Render::Texture>("Engine/cursor", "cursor"), "Can't load cursor");
			Verify(m_resource_manager.Load<Render::Texture>("Engine/engine_light_icon", "engine_light_icon"), "Can't load light icon");
			//Verify(m_resource_manager.Load<Render::Shader>("editor_cursor"), "Can't load cursor shader");
		}


		// Add default cubemap to resources
		Render::Texture* defaultCubeMapTexture = new Render::Texture();
		const auto cubePath = m_resource_manager.GetFilePath("Engine/cubemap", defaultCubeMapTexture);
		Verify(defaultCubeMapTexture->CreateCubeMapTexture(cubePath), "Can't create default cubemap");
		m_resource_manager.Add(defaultCubeMapTexture, "DefaultCubemap");


		return true;
	}

	inline Engine* Engine::GetEngine()
	{
		return m_engine;
	}

	inline ApplicationHandle* Engine::GetApplicationHandle()
	{
		return m_app_handle;
	}

	inline GameWindow* Engine::GetGameWindow()
	{
		return m_window;
	}

	inline GameTime Engine::GetGameTime()
	{
		return m_time;
	}

	inline Render::Renderer* Engine::GetRenderer()
	{
		return m_render;
	}

	inline InputManager* Engine::GetInputManager()
	{
		return m_input_m;
	}

	inline Scene::SceneManager* Engine::GetSceneManager()
	{
		return m_scene_manager;
	}

	inline Resources::ResourceManager& Engine::GetResourceManager()
	{
		return m_resource_manager;
	}

	inline bool Engine::isEditor()
	{
		return m_isEditor;
	}

	inline bool Engine::CheckAppParam(std::string param)
	{
		return m_params.find(param) != std::string::npos;
	}
}