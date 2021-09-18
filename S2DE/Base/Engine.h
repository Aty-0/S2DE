#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/Logger.h"
#include "Base/GameTime.h"

namespace S2DE
{
	class S2DE_API ApplicationHandle;
	class S2DE_API VisualConsole;
	class S2DE_API ResourceManager;
	class S2DE_API GameWindow;
	class S2DE_API Renderer;
	class S2DE_API InputManager;

	class S2DE_API Engine
	{
	public:
		Engine();
		~Engine();

		void RunGame(ApplicationHandle* app_handle, std::string pname = std::string());
		static void Destroy();

		static inline Engine* GetEngine() { return m_engine; }
		static inline ApplicationHandle* GetApplicationHandle() { return m_app_handle; }
		static inline GameWindow*		GetGameWindow() { return m_window; }
		static inline GameTime			GetGameTime() { return m_time; }
		static inline Renderer*			GetRenderer() { return m_render; }
		static inline InputManager*		GetInputManager() { return m_input_m; }
		static inline VisualConsole*	GetConsole() { return m_console; }
		static inline ResourceManager&	GetResourceManager() { return m_resource_manager; }
		static inline bool				isEditor() { return m_isEditor; }
	private:
		void		RunEngine(ApplicationHandle* app_handle, std::string pname, bool isEditor);

		void		UpdateInput();
		void		UpdateEngineInputKeys();
		void		RunLoop();
		void		OnLoop();
		void		OnGlobalUpdate(float DeltaTime);
		bool		LoadEngineResources();

		static Engine* m_engine;
		static ApplicationHandle* m_app_handle;
		static GameWindow* m_window;
		static GameTime m_time;
		static Renderer* m_render;
		static InputManager* m_input_m;
		static VisualConsole* m_console;
		static ResourceManager m_resource_manager;
		static bool m_isEditor;
	};
}