#pragma once
#include "Base/Main/Common.h"

#define S2DE_DEFAULT_WINDOW_WIDTH 1280
#define S2DE_DEFAULT_WINDOW_HEIGHT 768

static constexpr SDL_WindowFlags DefaultWindowFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE);

namespace S2DE::Core
{
	class S2DE_API GameWindow
	{
	public:
		GameWindow();
		~GameWindow();

		bool									Create(std::string name = std::string(), std::uint32_t w = S2DE_DEFAULT_WINDOW_WIDTH, std::uint32_t h = S2DE_DEFAULT_WINDOW_HEIGHT,
													std::int32_t x = 0, std::int32_t y = 0, SDL_WindowFlags flags = DefaultWindowFlags);											
		void									Destroy();		
		void									SetWidth(std::uint32_t width);
		void									SetHeight(std::uint32_t height);
		void									SetPositionX(std::int32_t x);
		void									SetPositionY(std::int32_t y);
		void									SetFullscreen(bool fullscreen, bool setClientRes = true);
		void									SetMouseVisible(bool visible);
		void									Restore();
		void									Hide();
		bool									PoolEvents();

		[[nodiscard]] inline DirectX::SimpleMath::Vector2		GetWindowPosition() const;

		[[nodiscard]] inline DirectX::SimpleMath::Vector2		GetResolution() const;
		[[nodiscard]] inline std::int32_t						GetWidth()  const;
		[[nodiscard]] inline std::int32_t						GetHeight() const;

		[[nodiscard]] inline std::int32_t						GetViewportWidth()  const;
		[[nodiscard]] inline std::int32_t						GetViewportHeight() const;



		[[nodiscard]] inline std::int32_t						GetPositionX() const;
		[[nodiscard]] inline std::int32_t						GetPositionY() const;
		[[nodiscard]] inline bool								isFullscreen() const;
		[[nodiscard]] inline bool								isCursorVisible() const;
		[[nodiscard]] inline bool								isActive() const;
		[[nodiscard]] inline HINSTANCE						GetInstance();
		[[nodiscard]] inline HWND&							GetHWND();
		[[nodiscard]] inline SDL_Window*						GetSDLWindow();
		[[nodiscard]] inline SDL_Event						GetEvent();

	private:
		SDL_Event   m_event;
		SDL_Window* m_window;
		bool		m_cursorVisible;

		void									ParseWindowEvents(SDL_Event event);
	};
}