#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/Logger.h"

namespace S2DE
{
	namespace Core
	{
		class ApplicationHandle;
		class GameWindow;
		class InputManager;
		class GameTime;

		namespace Resources
		{
			class ResourceManager;
		}
	}

	namespace Render
	{
		class Renderer;
	}

	namespace Scene
	{
		class SceneManager;
	}
}

using namespace S2DE::Core::Utils;

namespace S2DE::Core
{
	class S2DE_API Engine
	{
	public:
		Engine();
		~Engine();

		//Run engine in game mode
		void							RunEngineInGameMode(ApplicationHandle* app_handle, std::string pname = std::string());
		//Run engine in editor mode
		void							RunEngineInEditorMode(ApplicationHandle* app_handle);

		//Destroy engine and all components
		//Application will be closed when this function is called
		static void						Destroy();

		[[nodiscard]] static Engine* GetEngine();
		[[nodiscard]] static ApplicationHandle* GetApplicationHandle();
		[[nodiscard]] static GameWindow* GetGameWindow();
		[[nodiscard]] static GameTime			GetGameTime();
		[[nodiscard]] static Render::Renderer* GetRenderer();
		[[nodiscard]] static InputManager* GetInputManager();
		[[nodiscard]] static Scene::SceneManager* GetSceneManager();
		[[nodiscard]] static Resources::ResourceManager& GetResourceManager();
		[[nodiscard]] static bool isEditor();
		[[nodiscard]] static bool CheckAppParam(std::string param);

	private:
		// Run engine function
		// Initialize all engine components, load resources, etc...
		void							RunEngine(ApplicationHandle* app_handle, std::string pname, bool isEditor);

		// Update input manager and get key events from app
		void							UpdateInput();
		void							UpdateEngineInputKeys();

		// Run game loop
		void							RunLoop();
		void							OnLoop();
		void							OnGlobalUpdate(float DeltaTime);

		// Load engine resources
		// Configs, textures, shaders
		bool							LoadEngineResources();

		static Engine* m_engine;
		static ApplicationHandle* m_app_handle;
		static GameWindow* m_window;
		static GameTime m_time;
		static Render::Renderer* m_render;
		static InputManager* m_input_m;
		static Scene::SceneManager* m_scene_manager;
		static Resources::ResourceManager m_resource_manager;
		static bool m_isEditor;

		static std::string m_params;
	};
}