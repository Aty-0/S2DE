#include "InputManager.h"

#include "Base/Engine.h"
#include "Base/GameWindow.h"

namespace S2DE::Core
{
	InputManager::InputManager() :
		m_DirectInput(nullptr),
		m_InputKeyboard(nullptr),
		m_InputMouse(nullptr),
		m_KeyboardState(),
		m_KeyboardStateLast(),
		m_MouseCurrState(),
		m_MouseLastState()
	{

	}

	InputManager::~InputManager()
	{
		ReleaseInputDevice(m_InputMouse);
		ReleaseInputDevice(m_InputKeyboard);
		Release(m_DirectInput);
	}

	bool InputManager::IsMouseWheelUseForward() const
	{
		return m_MouseLastState.lZ > 0;
	}

	bool InputManager::IsMouseWheelUseBackward() const
	{
		return m_MouseLastState.lZ < 0;
	}

	bool InputManager::IsKeyPressed(Other::KeyCode keycode) const
	{
		return ((m_KeyboardState[(std::uint32_t)keycode] & 0x80) && (~m_KeyboardStateLast[(std::uint32_t)keycode] & 0x80)) ? true : false;
	}

	bool InputManager::IsKeyUp(Other::KeyCode keycode) const
	{
		return (m_KeyboardState[(std::uint32_t)keycode] & 0x80) ? false	: true;
	}

	bool InputManager::IsKeyDown(Other::KeyCode keycode) const
	{
		return (m_KeyboardState[(std::uint32_t)keycode] & 0x80) ? true : false;
	}

	bool InputManager::Update()
	{
		if (Engine::GetGameWindow()->isActive())
		{
			//Read the current state of the keyboard.
			if (m_lock_keyboard == false)
			{
				if (!ReadKeyboard())
					return false;
			}

			//Read the current state of the mouse.
			if (m_lock_mouse == false)
			{
				if (!ReadMouse())
					return false;
			}

			m_MouseLastState = m_MouseCurrState;
		}

		return true;
	}


	bool InputManager::ReadKeyboard()
	{
		if (m_InputKeyboard == nullptr)
			return false;

		for (std::int32_t i = 0; i < 256; i++)
			m_KeyboardStateLast[i] = m_KeyboardState[i];

		//Read the keyboard device.
		HRESULT result = m_InputKeyboard->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)&m_KeyboardState);
		if (FAILED(result))
		{
			//If the keyboard lost focus or was not acquired then try to get control back.
			if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
				m_InputKeyboard->Acquire();
			else
				return false;

		}

		return true;
	}

	bool InputManager::ReadMouse()
	{
		if (m_InputMouse == nullptr)
			return false;

		//Read the mouse device.
		HRESULT result = m_InputMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_MouseCurrState);

		if (FAILED(result))
		{
			//If the mouse lost focus or was not acquired then try to get control back.
			if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
				m_InputMouse->Acquire();
			else
				return false;

		}

		return true;
	}

	bool InputManager::Initialize()
	{
		Logger::Log("[InputManager] Initialize...");

		//Create DirectInput8 Interface
		if (FAILED(DirectInput8Create(Engine::GetGameWindow()->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, NULL)))
		{
			Logger::Error("Failed to create DirectInput8");
			return false;
		}

		//Create a keyboard device
		if (FAILED(m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_InputKeyboard, NULL)))
		{
			Logger::Error("Failed to create keyboard device");
			return false;
		}

		m_InputKeyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(m_InputKeyboard->SetCooperativeLevel(Engine::GetGameWindow()->isChild() == false ? Engine::GetGameWindow()->GetHWND() : NULL, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		{
			Logger::Error("Failed to set cooperative level for keyboard device");
			return false;
		}
	    m_InputKeyboard->Acquire();

		//Create a mouse device
		if (FAILED(m_DirectInput->CreateDevice(GUID_SysMouse, &m_InputMouse, NULL)))
		{
			Logger::Error("Failed to create mouse device");
			return false;
		}

		m_InputMouse->SetDataFormat(&c_dfDIMouse);
		if (FAILED(m_InputMouse->SetCooperativeLevel(Engine::GetGameWindow()->isChild() == false ? Engine::GetGameWindow()->GetHWND() : NULL, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		{
			Logger::Error("Failed to set cooperative level for mouse device");
			return false;
		}
		m_InputMouse->Acquire();


		return true;
	}

	void InputManager::LockMouseControl(bool lock)
	{
		m_lock_mouse = lock;
	}

	void InputManager::LockKeyboardControl(bool lock)
	{
		m_lock_keyboard = lock;
	}
}