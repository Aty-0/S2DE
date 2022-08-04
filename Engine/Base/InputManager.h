#pragma once
#include "Base/Main/Common.h"
#include "Base/Other/InputCode.h"

namespace S2DE::Core
{
	class S2DE_API InputManager
	{
	public:
		InputManager();
		~InputManager();

		// Update current sdl event
		void								Update();

		// Lock mouse control
		void								LockMouseControl(bool lock);
		// Lock keyboard control			
		void								LockKeyboardControl(bool lock);

		inline bool							IsMouseWheelTurnsForward()			 const;
		inline bool							IsMouseWheelTurnsBackward()			 const;

		inline bool							IsKeyPressed(Other::KeyCode keycode) const;
		inline bool							IsKeyUp(Other::KeyCode keycode)      const;
		inline bool							IsKeyDown(Other::KeyCode keycode)    const;

		//TODO: Mouse keys up, down, pressed

		inline DirectX::SimpleMath::Vector2 GetMousePositionRelative() const;
		inline DirectX::SimpleMath::Vector2 GetMousePositionGlobal() const;
		inline DirectX::SimpleMath::Vector2 GetMousePosition()		 const;
		inline DirectX::SimpleMath::Vector2 GetMouseWheelPosition()	 const;

		void								_MWheelUpdate(SDL_Event event);
		void								_MMotionUpdate(SDL_Event event);
		void								_KKeyDownArrayStateUpdate(SDL_Event event);
		void								_KKeyUpArrayStateUpdate(SDL_Event event);
		void								_MKeyUpArrayStateUpdate(SDL_Event event);
		void								_MKeyDownArrayStateUpdate(SDL_Event event);
	private:
		std::uint32_t						m_mouse;
		const std::uint8_t*					m_keyboard;
		bool								m_keyboardKeyUp[Other::KeyboardSize];
		bool								m_keyboardKeyDown[Other::KeyboardSize];
		bool								m_mouseKeyUp[Other::MouseSize];
		bool								m_mouseKeyDown[Other::MouseSize];

		bool								m_lock_mouse;
		bool								m_lock_keyboard;

		// On update 
		bool								m_isWheelTurnsForward;
		bool								m_isWheelTurnsBackward;
		DirectX::SimpleMath::Vector2		m_mRPMouseState;
		DirectX::SimpleMath::Vector2		m_mRCMouseState;
		DirectX::SimpleMath::Vector2		m_mRMouseState;
		DirectX::SimpleMath::Vector2		m_mWheel;
	};
}