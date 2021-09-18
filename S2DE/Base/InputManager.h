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

		bool					Initialize();
		bool					Update();

		DIMOUSESTATE			GetMouseCurrState() { return m_MouseCurrState; }
		DIMOUSESTATE			GetMouseLastState() { return m_MouseLastState; }

		inline bool				IsKeyPressed(KeyCode keycode) const;
		inline bool				IsKeyUp(KeyCode keycode)      const;
		inline bool				IsKeyDown(KeyCode keycode)    const;

	private:
		bool					ReadKeyboard();
		bool					ReadMouse();

	private:
		IDirectInput8*			m_DirectInput;
		//Device's
		IDirectInputDevice8*	m_InputKeyboard;
		IDirectInputDevice8*	m_InputMouse;

		std::uint8_t			m_KeyboardState[256];
		std::uint8_t			m_KeyboardStateLast[256];
		DIMOUSESTATE			m_MouseCurrState;
		DIMOUSESTATE			m_MouseLastState;

	};
}