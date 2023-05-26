#pragma once
#include "Base/Main/Common.h"
#include "Base/Other/InputCode.h"

namespace S2DE::Core
{
	class S2DE_API InputManager : public Core::Utils::Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();

		void LockMouseControl(bool lock);
		void LockKeyboardControl(bool lock);
		void LockWheel(bool lock);
		void Update();
		void Destroy();
			 
		void _MWheelUpdate(SDL_Event event);
		void _MMotionUpdate(SDL_Event event);
		void _KKeyDownArrayStateUpdate(SDL_Event event);
		void _KKeyUpArrayStateUpdate(SDL_Event event);
		void _MKeyUpArrayStateUpdate(SDL_Event event);
		void _MKeyDownArrayStateUpdate(SDL_Event event);

		[[nodiscard]] inline bool IsWheelLocked() const;
		[[nodiscard]] inline bool IsMouseLocked() const;
		[[nodiscard]] inline bool IsKeyboardLocked() const;
		[[nodiscard]] inline bool IsMouseWheelTurnsForward()			 const;
		[[nodiscard]] inline bool IsMouseWheelTurnsBackward()			 const;
		[[nodiscard]] inline bool IsKeyPressed(Other::KeyCode keycode) const;
		[[nodiscard]] inline bool IsKeyUp(Other::KeyCode keycode)      const;
		[[nodiscard]] inline bool IsKeyDown(Other::KeyCode keycode)    const;
		[[nodiscard]] inline bool IsMouseKeyPressed(Other::MouseKeyCode keycode) const;
		[[nodiscard]] inline bool IsMouseKeyUp(Other::MouseKeyCode keycode)      const;
		[[nodiscard]] inline bool IsMouseKeyDown(Other::MouseKeyCode keycode)    const;
		[[nodiscard]] inline Math::float2 GetMousePositionRelative() const;
		[[nodiscard]] inline Math::float2 GetMousePositionGlobal() const;
		[[nodiscard]] inline Math::float2 GetMousePosition()		 const;
		[[nodiscard]] inline Math::float2 GetMouseWheelPosition()	 const;

	private:
		std::uint32_t m_mouse;
		const std::uint8_t*	m_keyboard;
		bool m_keyboardKeyUp[Other::KeyboardSize];
		bool m_keyboardKeyDown[Other::KeyboardSize];
		bool m_mouseKeyUp[Other::MouseSize];
		bool m_mouseKeyDown[Other::MouseSize];
		bool m_lock_wheel;
		bool m_lock_mouse;
		bool m_lock_keyboard;
		bool m_isMouseMotionWork;
		bool m_isMouseWheelMotionWork;
		bool m_isWheelTurnsForward;
		bool m_isWheelTurnsBackward;
		Math::float2 m_mRPMouseState;
		Math::float2 m_mRCMouseState;
		Math::float2 m_mRMouseState;
		Math::float2 m_mPosition;
		Math::float2 m_mGlobalPosition;
		Math::float2 m_mWheel;
	};
}