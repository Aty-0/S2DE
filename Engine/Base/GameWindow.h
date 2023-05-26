#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/Callback.h" // FIX ME


namespace S2DE::Core
{
	class S2DE_API GameWindow : public Core::Utils::Singleton<GameWindow>
	{
	public:
		GameWindow();
		~GameWindow();

		Core::Utils::Callback<Core::Utils::DefaultCallbackFunction>		onWindowResized;
		static constexpr auto DefaultWindowWidth = 1280;
		static constexpr auto DefaultWindowHeight = 768;
		static constexpr auto DefaultWindowFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE);

		bool Create(std::string name = std::string(), std::uint32_t w = DefaultWindowWidth, std::uint32_t h = DefaultWindowHeight,
			 	std::int32_t x = 0, std::int32_t y = 0, SDL_WindowFlags flags = DefaultWindowFlags);											
		void Destroy();		
		void SetWidth(std::uint32_t width);
		void SetHeight(std::uint32_t height);
		void SetPositionX(std::int32_t x);
		void SetPositionY(std::int32_t y);
		void SetFullscreen(bool fullscreen, bool setClientRes = true);
		void SetMouseVisible(bool visible);
		void Restore();
		void Hide();
		bool PoolEvents();

		[[nodiscard]] inline Math::float2 GetWindowPosition() const;
		[[nodiscard]] inline Math::float2 GetResolution() const;
		[[nodiscard]] inline std::int32_t GetWidth()  const;
		[[nodiscard]] inline std::int32_t GetHeight() const;
		[[nodiscard]] inline std::int32_t GetViewportWidth()  const;
		[[nodiscard]] inline std::int32_t GetViewportHeight() const;
		[[nodiscard]] inline std::int32_t GetPositionX() const;
		[[nodiscard]] inline std::int32_t GetPositionY() const;
		[[nodiscard]] inline bool isFullscreen() const;
		[[nodiscard]] inline bool isCursorVisible() const;
		[[nodiscard]] inline bool isActive() const;
		[[nodiscard]] inline HINSTANCE	 GetHInstance();
		[[nodiscard]] inline HWND& GetHWND();
		[[nodiscard]] inline SDL_Window* GetSDLWindow();
		[[nodiscard]] inline SDL_Event GetEvent();

	private:
		void ParseWindowEvents(SDL_Event event);

		SDL_Event   m_event;
		SDL_Window* m_window;
		bool		m_cursorVisible;
	};
}