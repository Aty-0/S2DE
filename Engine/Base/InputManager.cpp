#include "InputManager.h"

#include "Base/Engine.h"
#include "Base/GameWindow.h"

namespace S2DE::Core
{
	InputManager::InputManager() 
	{

	}

	InputManager::~InputManager()
	{

	}
	void InputManager::_MWheelUpdate(SDL_Event event)
	{
		m_mWheel = DirectX::SimpleMath::Vector2(static_cast<float>(event.wheel.x), static_cast<float>(event.wheel.y));
		m_isWheelTurnsForward	=  event.wheel.preciseY >= 1;
		m_isWheelTurnsBackward	=  event.wheel.preciseY <= -1;
	}

	void InputManager::_MMotionUpdate(SDL_Event event)
	{
		// FIX ME: When vsync is on we are get ragged movements
		 
		// Two ways how we can get relative mouse state
		// First:
		m_mRPMouseState = m_mRCMouseState;		
		m_mRCMouseState = DirectX::SimpleMath::Vector2(event.motion.x, event.motion.y);		
		m_mRMouseState = m_mRCMouseState - m_mRPMouseState;

		// Second:
		//m_mRMouseState = DirectX::SimpleMath::Vector2(event.motion.xrel, event.motion.yrel);

		// Get global mouse position
		std::int32_t x = 0, y = 0;
		SDL_GetGlobalMouseState(&x, &y);
		m_mGlobalPosition = DirectX::SimpleMath::Vector2(static_cast<float>(x), static_cast<float>(y));

		// Get mouse position relative to window
		m_mPosition = DirectX::SimpleMath::Vector2(static_cast<float>(event.motion.x), static_cast<float>(event.motion.y));
	}

	void InputManager::Update()
	{
		// Disable all keys

		m_isWheelTurnsForward = false;
		m_isWheelTurnsBackward = false;

		for (auto& up : m_keyboardKeyUp)
			up = false;

		for (auto& down : m_keyboardKeyDown)
			down = false;

		for (auto& up : m_mouseKeyUp)
			up = false;

		for (auto& down : m_mouseKeyDown)
			down = false;
	}

	void InputManager::_KKeyDownArrayStateUpdate(SDL_Event event)
	{
		m_keyboard = SDL_GetKeyboardState(nullptr);
		m_keyboardKeyDown[event.key.keysym.scancode] = true;
	}

	void InputManager::_KKeyUpArrayStateUpdate(SDL_Event event)
	{
		m_keyboard = SDL_GetKeyboardState(nullptr);
		m_keyboardKeyUp[event.key.keysym.scancode] = true;
	}

	void InputManager::_MKeyUpArrayStateUpdate(SDL_Event event)
	{
		m_mouse = SDL_GetMouseState(&event.motion.x, &event.motion.y);
		m_mouseKeyUp[event.button.button] = true;
	}

	void InputManager::_MKeyDownArrayStateUpdate(SDL_Event event)
	{
		m_mouse = SDL_GetMouseState(&event.motion.x, &event.motion.y);
		m_mouseKeyDown[event.button.button] = true;
	}

	DirectX::SimpleMath::Vector2 InputManager::GetMousePositionRelative() const
	{	
		SDL_CaptureMouse(SDL_TRUE);
		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_WarpMouseInWindow(Engine::GetGameWindow()->GetSDLWindow(), Engine::GetGameWindow()->GetWidth() / 2, Engine::GetGameWindow()->GetHeight() / 2);
		return m_mRMouseState;
	}

	DirectX::SimpleMath::Vector2 InputManager::GetMousePositionGlobal() const
	{
		return m_mGlobalPosition;
	}

	DirectX::SimpleMath::Vector2 InputManager::GetMousePosition() const
	{
		return m_mPosition;
	}

	DirectX::SimpleMath::Vector2 InputManager::GetMouseWheelPosition() const
	{
		return m_mWheel;
	}

	bool InputManager::IsMouseWheelTurnsForward() const
	{
		return m_isWheelTurnsForward && !m_lock_wheel;
	}

	bool InputManager::IsMouseWheelTurnsBackward() const
	{
		return m_isWheelTurnsBackward && !m_lock_wheel;
	}

	bool InputManager::IsMouseKeyPressed(Other::MouseKeyCode keycode) const
	{
		if (m_mouseKeyDown == nullptr || m_lock_mouse)
			return false;

		std::uint8_t i = static_cast<std::int8_t>(keycode);
		return m_mouseKeyDown[i];
	}

	bool InputManager::IsMouseKeyUp(Other::MouseKeyCode keycode)      const
	{
		if (m_mouseKeyUp == nullptr || m_lock_mouse)
			return false;

		std::uint8_t i = static_cast<std::uint8_t>(keycode);
		return m_mouseKeyUp[i];
	}

	bool InputManager::IsMouseKeyDown(Other::MouseKeyCode keycode)    const
	{
		if (m_lock_mouse)
			return false;

		std::uint8_t i = static_cast<std::uint8_t>(keycode);
		if (m_mouse & SDL_BUTTON(i))
			return true;

		return false;
	}

	bool InputManager::IsKeyPressed(Other::KeyCode keycode) const
	{		
		if (m_keyboardKeyDown == nullptr || m_lock_keyboard)
			return false;

		std::uint16_t i = static_cast<std::int16_t>(keycode);
		return m_keyboardKeyDown[i];
	}

	bool InputManager::IsKeyUp(Other::KeyCode keycode) const
	{		
		if (m_keyboardKeyUp == nullptr || m_lock_keyboard)
			return false;

		std::uint16_t i = static_cast<std::uint16_t>(keycode);
		return m_keyboardKeyUp[i];
	}

	bool InputManager::IsKeyDown(Other::KeyCode keycode) const
	{
		if (m_keyboard == nullptr || m_lock_keyboard)
			return false;

		std::uint16_t i = static_cast<std::uint16_t>(keycode);
		if (m_keyboard[i])
			return true;
	
		return false;
	}

	bool InputManager::IsMouseLocked() const
	{	 
		return m_lock_mouse;
	}	 
		 
	bool InputManager::IsKeyboardLocked() const
	{
		return m_lock_keyboard;
	}

	void InputManager::LockMouseControl(bool lock)
	{
		m_lock_mouse = lock;
	}

	void InputManager::LockKeyboardControl(bool lock)
	{
		m_lock_keyboard = lock;
	}

	void InputManager::LockWheel(bool lock)
	{
		m_lock_wheel = lock;
	}

	bool InputManager::IsWheelLocked() const
	{
		return m_lock_wheel;
	}
}