#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/Logger.h"
#include "Base/GameTime.h"

namespace S2DE
{
	namespace Core
	{
		class ApplicationHandle;
		class ResourceManager;
		class GameWindow;
		class InputManager;
		namespace Debug
		{
			class VisualConsole;
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
		void							RunEngineInGameMode(ApplicationHandle* app_handle, std::string pname = std::string(), std::string params = std::string());
		//Run engine in editor mode
		void							RunEngineInEditorMode(ApplicationHandle* app_handle, std::string params = std::string());

		//Destroy engine and all components
		//Application will be closed when this function is called
		static void						Destroy();

		static inline Engine*			GetEngine() { return m_engine; }
		static inline ApplicationHandle* GetApplicationHandle() { return m_app_handle; }
		static inline GameWindow*		GetGameWindow() { return m_window; }
		static inline GameTime			GetGameTime() { return m_time; }
		static inline Render::Renderer*			GetRenderer() { return m_render; }
		static inline InputManager*		GetInputManager() { return m_input_m; }
		static inline Debug::VisualConsole*	GetConsole() { return m_console; }
		static inline Scene::SceneManager*		GetSceneManager() { return m_scene_manager; }
		static inline ResourceManager&	GetResourceManager() { return m_resource_manager; }
		static inline bool				isEditor() { return m_isEditor; }

		static inline bool				CheckAppParam(std::string param) { return m_params.find(param) != std::string::npos; }

	private:
		//Run engine function
		//Initialize all engine components, load resources, etc...
		void							RunEngine(ApplicationHandle* app_handle, std::string pname, bool isEditor);

		//Update input manager and get key events from app
		void							UpdateInput();
		void							UpdateEngineInputKeys();

		//Run game loop
		void							RunLoop();
		void							OnLoop();
		void							OnGlobalUpdate(float DeltaTime);

		//Load engine resources
		//Configs, textures, shaders
		bool							LoadEngineResources();

		static Engine* m_engine;
		static ApplicationHandle* m_app_handle;
		static GameWindow* m_window;
		static GameTime m_time;
		static Render::Renderer* m_render;
		static InputManager* m_input_m;
		static Debug::VisualConsole* m_console;
		static Scene::SceneManager* m_scene_manager;
		static ResourceManager m_resource_manager;
		static bool m_isEditor;

		static std::string m_params;
	};
}