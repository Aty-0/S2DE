#pragma once
#include "Base/Main/Common.h"
#include "Base/Other/InputCode.h"

//Include direct input header
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace S2DE
{
	class S2DE_API InputManager
	{
	public:
		InputManager();
		~InputManager();

		inline bool				IsKeyPressed(KeyCode keycode) const;
		inline bool				IsKeyUp(KeyCode keycode)      const;
		inline bool				IsKeyDown(KeyCode keycode)    const;

		inline DIMOUSESTATE		GetMouseCurrState() const { return m_MouseCurrState; }
		inline DIMOUSESTATE		GetMouseLastState() const { return m_MouseLastState; }

		bool					Initialize();
		bool					Update();

	private:
		bool					ReadKeyboard();
		bool					ReadMouse();
		IDirectInput8*			m_DirectInput;
		IDirectInputDevice8*	m_InputKeyboard;
		IDirectInputDevice8*	m_InputMouse;
		std::uint8_t			m_KeyboardState[256];
		std::uint8_t			m_KeyboardStateLast[256];
		DIMOUSESTATE			m_MouseCurrState;
		DIMOUSESTATE			m_MouseLastState;

	};
}