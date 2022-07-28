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
		inline DirectX::SimpleMath::Vector2		GetResolution() const;
		inline DirectX::SimpleMath::Vector2		GetWindowPosition() const;
		inline std::int32_t						GetWidth()  const;
		inline std::int32_t						GetHeight() const;
		inline std::int32_t						GetPositionX() const;
		inline std::int32_t						GetPositionY() const;
		inline bool								isFullscreen() const;
		inline bool								isCursorVisible() const;
		inline bool								isActive() const;
		inline HINSTANCE						GetInstance();
		inline HWND&							GetHWND();
		inline SDL_Window*						GetSDLWindow();

	private:
		SDL_Window* m_window;
		bool		m_cursorVisible;

	};
}