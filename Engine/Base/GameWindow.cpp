#include "GameWindow.h"
#include "Base/Engine.h"
#include "Base/ApplicationHandle.h"
#include "Base/Main/BuildDate.h"
#include "Render/Renderer.h"

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

	bool GameWindow::PoolEvents()
	{
		SDL_Event e;
		if (SDL_PollEvent(&e) != 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type)
			{
				case SDL_EventType::SDL_WINDOWEVENT:
				{
					//TODO: Handle more events
					if (e.window.event == SDL_WINDOWEVENT_RESIZED)
						Core::Engine::GetRenderer()->Reset();

					break;
				}

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
	}

	void GameWindow::SetMouseVisible(bool visible)
	{
		if (m_cursorVisible != visible) // Don't call any function if we are have same parameter state 
		{
			m_cursorVisible = visible;
			SDL_ShowCursor(visible);
			SDL_WarpMouseInWindow(m_window, GetWidth() / 2, GetHeight() / 2);
		}
	}

	void GameWindow::Destroy()
	{
		Logger::Log("[SDL] Destroy game window...");
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

	DirectX::SimpleMath::Vector2 GameWindow::GetResolution() const 
	{ 
		std::int32_t w = 0, h = 0; 
		SDL_GetWindowSize(m_window, &w, &h); 
		return DirectX::SimpleMath::Vector2(static_cast<float>(w), static_cast<float>(h));
	}

	DirectX::SimpleMath::Vector2 GameWindow::GetWindowPosition() const 
	{ 
		std::int32_t x = 0, y = 0; 
		SDL_GetWindowPosition(m_window, &x, &y); 
		return DirectX::SimpleMath::Vector2(static_cast<float>(x), static_cast<float>(y)); 
	}

	std::int32_t GameWindow::GetWidth()  const 
	{
		return static_cast<std::int32_t>(GetResolution().x); 
	}

	std::int32_t GameWindow::GetHeight() const 
	{
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
}