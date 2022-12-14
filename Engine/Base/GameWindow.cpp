#include "GameWindow.h"
#include "Base/Engine.h"
#include "Base/InputManager.h"
#include "Base/Main/BuildDate.h"

#include "Render/Renderer.h"
#include "Editor/EditorRenderWindow.h"

namespace S2DE::Core
{
	GameWindow::GameWindow() 
	{

	}


	GameWindow::~GameWindow()
	{

	}

	bool GameWindow::Create(std::string name, std::uint32_t w, std::uint32_t h, std::int32_t x, std::int32_t y, SDL_WindowFlags flags)
	{
		Logger::Log("[SDL] Create game window...");
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			Logger::Error("[SDL] Error Desc: %s", SDL_GetError());
			S2DE_FATAL_ERROR("[SDL] Can't initilize video!");
			return false;
		}

		SDL_version ver = { };
		SDL_GetVersion(&ver);
		Logger::Log("[SDL] SDL Version %d.%d.%d", ver.major, ver.minor, ver.patch);

		std::string str = std::string();

#ifdef NDEBUG
		str = Core::Utils::isStringEmpty(name) ? "S2DE " + std::string(S2DE_BUILD_DATE) : name;

		if (Engine::isEditor())
			str.append(" Editor");

#else
		str = "S2DE " + std::string(S2DE_BUILD_DATE);

		if (Engine::isEditor())
			str.append(" Editor");

		if (!Core::Utils::isStringEmpty(name))
			str.append(std::string(" (") + name + ")");

		str.append(" Debug");
#endif

		m_window = SDL_CreateWindow(str.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags);

		if (m_window == nullptr)
		{
			Logger::Error("[SDL] Error Desc: %s", SDL_GetError());
			S2DE_FATAL_ERROR("[SDL] Can't initilize window!");
			return false;
		}
		
		return true;
	}

	void GameWindow::ParseWindowEvents(SDL_Event event)
	{
		switch (event.window.event)
		{		
			case SDL_WindowEventID::SDL_WINDOWEVENT_DISPLAY_CHANGED: // It's needed ?
			case SDL_WindowEventID::SDL_WINDOWEVENT_RESIZED:
			case SDL_WindowEventID::SDL_WINDOWEVENT_SIZE_CHANGED:
			case SDL_WindowEventID::SDL_WINDOWEVENT_MAXIMIZED:
				Core::Engine::GetRenderer()->Reset();
				if (Engine::isEditor() == false)
				{
					Core::Engine::GetRenderer()->UpdateViewport();
				}
				break;			
		}
	}

	bool GameWindow::PoolEvents()
	{
		Core::Engine::GetInputManager()->Update();

		while (SDL_PollEvent(&m_event) != 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&m_event);

			switch (m_event.type)
			{		
				// Parse window events
				case SDL_EventType::SDL_WINDOWEVENT:
					ParseWindowEvents(m_event);
					break;
				// TODO: It's not all control events 
				// TODO: joystick support
				// Call event update when we are get the control type event
				case SDL_EventType::SDL_TEXTINPUT:
					break;
				case SDL_EventType::SDL_MOUSEWHEEL:
					Core::Engine::GetInputManager()->_MWheelUpdate(m_event);
					break;
				case SDL_EventType::SDL_MOUSEMOTION:
					Core::Engine::GetInputManager()->_MMotionUpdate(m_event);
					break;
				case SDL_EventType::SDL_MOUSEBUTTONDOWN:
					// Process the all keys on mouseDown array
					Core::Engine::GetInputManager()->_MKeyDownArrayStateUpdate(m_event);
					break;
				case SDL_EventType::SDL_MOUSEBUTTONUP:
					// Process the all keys on mouseUp array
					Core::Engine::GetInputManager()->_MKeyUpArrayStateUpdate(m_event);
					break;					
				case SDL_EventType::SDL_KEYDOWN:
					// Process the all keys on keyDown array
					Core::Engine::GetInputManager()->_KKeyDownArrayStateUpdate(m_event);
					break;
				case SDL_EventType::SDL_KEYUP:
					// Process the all keys on keyUp array
					Core::Engine::GetInputManager()->_KKeyUpArrayStateUpdate(m_event);
					break;

				// Application close
				case SDL_EventType::SDL_QUIT:
					return false;
			}
		}
		return true;
	}

	void GameWindow::Restore()
	{
		SDL_RestoreWindow(m_window);
	}

	void GameWindow::Hide()
	{
		SDL_MinimizeWindow(m_window);
	}

	void GameWindow::SetWidth(std::uint32_t width)
	{
		SDL_SetWindowSize(m_window, width, GetHeight());
	}

	void GameWindow::SetHeight(std::uint32_t height)
	{
		SDL_SetWindowSize(m_window, GetWidth(), height);
	}

	void GameWindow::SetPositionX(std::int32_t x)
	{
		SDL_SetWindowPosition(m_window, x, GetPositionY());
	}

	void GameWindow::SetPositionY(std::int32_t y)
	{
		SDL_SetWindowPosition(m_window, GetPositionX(), y);
	}

	void GameWindow::SetFullscreen(bool fullscreen, bool setClientRes)
	{
		if (setClientRes)
		{
			SDL_DisplayMode displayMode = { };
			S2DE_ASSERT(SDL_GetCurrentDisplayMode(0, &displayMode) == 0);
			SDL_SetWindowSize(m_window, displayMode.w, displayMode.h);
		}

		std::uint32_t flags = fullscreen == true ? SDL_WINDOW_FULLSCREEN : false;
		SDL_SetWindowFullscreen(m_window, flags);

		Core::Engine::GetRenderer()->Reset();
		Core::Engine::GetRenderer()->UpdateViewport();
	}

	void GameWindow::SetMouseVisible(bool visible)
	{
		if (m_cursorVisible != visible) // Don't call any function if we are have same parameter state 
		{
			m_cursorVisible = visible;
			SDL_ShowCursor(m_cursorVisible);
			SDL_WarpMouseInWindow(m_window, GetWidth() / 2, GetHeight() / 2);			

			if(m_cursorVisible)
				SDL_SetRelativeMouseMode(SDL_FALSE);
		}
	}

	void GameWindow::Destroy()
	{
		Logger::Log("[SDL] Destroy game window...");
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}


	DirectX::SimpleMath::Vector2 GameWindow::GetWindowPosition() const 
	{ 
		std::int32_t x = 0, y = 0; 
		SDL_GetWindowPosition(m_window, &x, &y); 
		return DirectX::SimpleMath::Vector2(static_cast<float>(x), static_cast<float>(y)); 
	}

	DirectX::SimpleMath::Vector2 GameWindow::GetResolution() const 
	{
		std::int32_t w = 0, h = 0;
		SDL_GetWindowSize(m_window, &w, &h);
		return DirectX::SimpleMath::Vector2(static_cast<float>(w), static_cast<float>(h));
	}

	std::int32_t GameWindow::GetWidth()  const 
	{
		return static_cast<std::int32_t>(GetResolution().x);
	}

	std::int32_t GameWindow::GetHeight() const 
	{
		return static_cast<std::int32_t>(GetResolution().y);
	}

	std::int32_t GameWindow::GetViewportWidth()  const
	{
		if (Engine::isEditor())
		{
			const auto renderWindow = Engine::GetRenderer()->GetImGui_Window<Editor::EditorRenderWindow*>("EditorRenderWindow");
			if (renderWindow != nullptr)
			{
				return static_cast<std::int32_t>(renderWindow->GetWindowWidth());
			}
		}

		return static_cast<std::int32_t>(GetResolution().x);
	}

	std::int32_t GameWindow::GetViewportHeight() const
	{
		if (Engine::isEditor())
		{
			const auto renderWindow = Engine::GetRenderer()->GetImGui_Window<Editor::EditorRenderWindow*>("EditorRenderWindow");
			if (renderWindow != nullptr)
			{
				return static_cast<std::int32_t>(renderWindow->GetWindowHeight());
			}
		}

		return static_cast<std::int32_t>(GetResolution().y);
	}

	std::int32_t GameWindow::GetPositionX() const 
	{ 
		return  static_cast<std::int32_t>(GetWindowPosition().x); 
	}

	std::int32_t GameWindow::GetPositionY() const
	{
		return  static_cast<std::int32_t>(GetWindowPosition().x);
	}

	bool GameWindow::isFullscreen() const 
	{ 
		std::uint32_t flags = SDL_GetWindowFlags(m_window); 
		return (flags & SDL_WindowFlags::SDL_WINDOW_FULLSCREEN) || (flags & SDL_WindowFlags::SDL_WINDOW_FULLSCREEN_DESKTOP); 
	}

	bool GameWindow::isCursorVisible() const 
	{ 
		return m_cursorVisible;
	}

	bool GameWindow::isActive() const 
	{ 
		std::uint32_t flags = SDL_GetWindowFlags(m_window); 
		return (flags & SDL_WindowFlags::SDL_WINDOW_INPUT_FOCUS) || (flags & SDL_WindowFlags::SDL_WINDOW_MOUSE_FOCUS); 
	}
	
	HINSTANCE GameWindow::GetInstance() 
	{ 
		return GetModuleHandle(NULL); 
	}

	HWND& GameWindow::GetHWND() 
	{ 
		SDL_SysWMinfo sysWMInfo = { }; 
		SDL_VERSION(&sysWMInfo.version); 
		SDL_GetWindowWMInfo(m_window, &sysWMInfo); 
		return sysWMInfo.info.win.window; 
	}

	SDL_Window* GameWindow::GetSDLWindow() 
	{ 
		return m_window; 
	}
	
	SDL_Event GameWindow::GetEvent()
	{
		return m_event;
	}
}