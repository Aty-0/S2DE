#pragma once
#include "Base/Main/Common.h"
#include "Base/GameWindow.h"
#include "Base/GameTime.h"
#include "Base/Utils/Logger.h"
#include "Graphics/Renderer.h"

namespace S2DE
{
	class ApplicationHandle;
	
	class S2DE_DX11_API Engine
	{
	public:
		Engine();
		~Engine();

		void RunGame(ApplicationHandle* app_handle, std::string pname = std::string());
		static void Destroy();

		static inline Engine* GetEngine() { return m_engine; }
		static inline ApplicationHandle* GetApplicationHandle() { return m_app_handle; }
		static inline GameWindow* GetGameWindow() { return m_window; }
		static inline GameTime  GetGameTime() { return m_time; }
		static inline Renderer* GetRenderer() { return m_render; }
		static inline bool	isEditor() { return m_isEditor; }
	private:
		void		RunEngine(ApplicationHandle* app_handle, std::string pname, bool isEditor);

		void		RunLoop();
		void		OnLoop();
		bool		LoadEngineResources();

		static Engine* m_engine;
		static ApplicationHandle* m_app_handle;
		static GameWindow* m_window;
		static GameTime m_time;
		static Renderer* m_render;
		static bool m_isEditor;
	};
}