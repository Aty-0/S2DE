#include "Engine.h"

#include "Base/GameTime.h"
#include "Base/DebugTools/Debug_Info.h"
#include "Base/Other/SplashScreen.h"
#include "Base/ApplicationHandle.h"
#include "Base/GameWindow.h"
#include "Base/InputManager.h"
#include "Base/ResourceManager.h"

#include "Render/Renderer.h"
#include "Render/ImGui_Window.h"

// resources
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/Mesh.h"
#include "Render/Font.h"

#include "Scene/SceneManager.h"


using namespace S2DE::Core::Other;

#define S2DE_EXIT_PROCESS() exit(EXIT_SUCCESS);

namespace S2DE::Core
{
	bool Engine::m_isEditor = false;

	Engine::Engine() 
		: m_applicationHandle(nullptr)
	{

	}

	Engine::~Engine()
	{
		Delete(m_applicationHandle);
	}

	void Engine::RunEngine(ApplicationHandle* applicationHandle, bool isEditor)
	{
		m_isEditor = isEditor;

		// Get params
		m_params = GetCommandLineA();

		const bool showSplashWindow = !FindAppParam("-nsplash");

		// TODO: load project name from project.json
		SplashScreen* sp = nullptr;
		SplashScreen::SetProjectName("Untitled");

		if (showSplashWindow)
		{
			sp = new SplashScreen();
			Assert(sp->ShowSplashScreen(GetModuleHandle(NULL)), "");
		}

		// Create log file 
		Utils::Logger::CreateLogFile();
		Utils::Logger::Log("Starting engine...");

		SplashScreen::SetLoadState("Starting engine...", sp);

		// Check application handle
		if ((m_applicationHandle = applicationHandle) == nullptr)
		{
			S2DE_FATAL_ERROR("No application handle setted");
			return;
		}

		SplashScreen::SetLoadState("Create game window...", sp);
		const auto window = Core::GameWindow::GetInstance();
		window->Create("Untitled");

		if (showSplashWindow)
		{
			window->Hide();
		}


		SplashScreen::SetLoadState("Initialize render...", sp);
		const auto renderer = Render::Renderer::GetInstance();
		if (!renderer->Create())
		{
			return;
		}

		// Load engine resources, read main config, etc
		SplashScreen::SetLoadState("Load engine resources...", sp);
		
		if (!LoadEngineResources())
		{
			S2DE_FATAL_ERROR("Failed to load engine resources!");
			return;
		}

		SplashScreen::SetLoadState("Initialize scene...", sp);
		const auto sceneManager = Scene::SceneManager::GetInstance();
		sceneManager->CreateNewScene();

		SplashScreen::SetLoadState("Load game resources...", sp);
		if (!m_applicationHandle->LoadResources())
		{
			S2DE_FATAL_ERROR("Failed to load application resources!");
			return;
		}

		// Application On start
		Utils::Logger::Log("Application OnStart event");
		m_applicationHandle->OnStart();


		// Destroy splash screen and show game window
		if (showSplashWindow)
		{
			sp->Close();
			Delete(sp);
		}

		window->Restore();

		// FIX ME: Move to scene ?
		const auto debug = Core::Debug::Debug_Info::GetInstance();
		debug->CreateDbgTexts();

		// First time when we are call callback
		window->onWindowResized.RunAllCallbacks();
		
		// Run main game loop
		RunLoop();
	}

	void Engine::RunLoop()
	{
		static const auto window = Core::GameWindow::GetInstance();

		while (window->PoolEvents())
		{
			OnLoop();
		}
	}

	void Engine::UpdateEngineInputKeys()
	{
		static const auto inputManager = Core::InputManager::GetInstance();
		static const auto renderer = Render::Renderer::GetInstance();
		static const auto window = Core::GameWindow::GetInstance();

		if (inputManager->IsKeyPressed(KeyCode::KEY_ESCAPE))
		{
			Destroy();
		}

		if (inputManager->IsKeyPressed(KeyCode::KEY_GRAVE))
		{
			renderer->GetImGui_Window<Render::ImGui_Window*>("Console")->ToggleDraw();
		}

		if (inputManager->IsKeyPressed(KeyCode::KEY_F11))
		{
			window->SetFullscreen(!window->isFullscreen());
		}

		if (inputManager->IsKeyPressed(KeyCode::KEY_0))
		{
			const auto debug = Core::Debug::Debug_Info::GetInstance();
			debug->ToggleDraw();
		}

#ifdef S2DE_DEBUG_RENDER_MODE
		//if (Engine::isEditor())
		{
			if (inputManager->IsKeyPressed(KeyCode::KEY_F9))
			{
				renderer->ToggleImGuiDemoWindowVisible();
			}
		}
#endif

		if (Engine::isEditor())
		{
			if (inputManager->IsKeyPressed(KeyCode::KEY_F1))
			{
				renderer->ToggleImGuiWindowsVisible();
			}
		}

	}

	void Engine::UpdateInput()
	{
		m_applicationHandle->InputEvents();
		UpdateEngineInputKeys();
	}

	void Engine::OnGlobalUpdate(float DeltaTime)
	{
		static const auto sceneManager = Scene::SceneManager::GetInstance();

		sceneManager->UpdateScene(DeltaTime);
		m_applicationHandle->OnUpdate(DeltaTime);

		static const auto debug = Core::Debug::Debug_Info::GetInstance();
		debug->UpdateTexts();
	}

	void Engine::OnLoop()
	{
		static const auto time = Core::GameTime::GetInstance();

		time->Begin();
		OnGlobalUpdate(time->GetDeltaTime());

		static const auto renderer = Render::Renderer::GetInstance();
		renderer->Render();

		UpdateInput();		
		time->End();
	}

	void Engine::Destroy()
	{
		Core::Utils::Logger::Log("Destroying engine...");

		m_applicationHandle->OnClose();

		const auto sceneManager = Scene::SceneManager::GetInstance();
		sceneManager->Clear();

		const auto resourceManager = Core::Resources::ResourceManager::GetInstance();
		resourceManager->ClearAll();

		const auto renderer = Render::Renderer::GetInstance();
		renderer->Destroy();

		const auto inputManager = Core::InputManager::GetInstance();
		inputManager->Destroy();

		S2DE_EXIT_PROCESS();
	}

	// TODO: Make something like json file with resource name, type, etc, to load 

	bool Engine::LoadEngineResources()
	{
		const auto resourceManager = Core::Resources::ResourceManager::GetInstance();
		// Load default texture
		Verify(resourceManager->Load<Render::Texture>("Engine/DefaultTexture", Core::Resources::ResourceManager::DefaultResourceName, true), "Can't load default texture");
		Verify(resourceManager->Load<Render::Texture>("Engine/DefaultSky", "DefaultSky", true), "Can't load default sky texture");
		// Load shaders 
		Verify(resourceManager->Load<Render::Shader>("Sprite", std::string(), true), "Can't load sprite shader");
		Verify(resourceManager->Load<Render::Shader>("Line", std::string(), true), "Can't load line shader");
		Verify(resourceManager->Load<Render::Shader>("Mesh", std::string(), true), "Can't load mesh shader");
		Verify(resourceManager->Load<Render::Shader>("Text", std::string(), true), "Can't load text shader");
		Verify(resourceManager->Load<Render::Shader>("Skybox", std::string(), true), "Can't load skybox shader");
		
		// Load editor resources 
		if (Engine::isEditor())
		{
			Verify(resourceManager->Load<Render::Texture>("Engine/cursor", "cursor", true), "Can't load cursor");
			Verify(resourceManager->Load<Render::Texture>("Engine/engine_light_icon", "engine_light_icon", true), "Can't load light icon");
			Verify(resourceManager->Load<Render::Texture>("Engine/engine_game_object", "engine_game_object", true), "Can't load game object icon");
			//Verify(m_resource_manager.Load<Render::Shader>("editor_cursor"), "Can't load cursor shader");
		}


		// Add default cubemap to resources
		Render::Texture* defaultCubeMapTexture = new Render::Texture();
		const auto cubePath = resourceManager->GetFilePath("Engine/cubemap", defaultCubeMapTexture);
		Verify(defaultCubeMapTexture->CreateCubeMapTexture(cubePath), "Can't create default cubemap");
		resourceManager->Add(defaultCubeMapTexture, "DefaultCubemap", true);


		return true;
	}

	ApplicationHandle* Engine::GetApplicationHandle() const
	{
		return m_applicationHandle;
	}

	bool Engine::isEditor() noexcept
	{
		return m_isEditor;
	}

	inline bool Engine::FindAppParam(std::string param) const
	{
		return m_params.find(param) != std::string::npos;
	}
}