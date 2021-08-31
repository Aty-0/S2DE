#include "InputManager.h"

#include "Base/Engine.h"
#include "Base/GameWindow.h"

namespace S2DE
{
	InputManager::InputManager()
	{
		m_DirectInput = NULL;
		m_InputKeyboard = NULL;
		m_InputMouse = NULL;
	}

	InputManager::~InputManager()
	{
		ReleaseInputDevice(m_InputMouse);
		ReleaseInputDevice(m_InputKeyboard);
		Release(m_DirectInput);
	}

	bool InputManager::IsKeyPressed(KeyCode keycode) const
	{
		return ((m_KeyboardState[(std::uint32_t)keycode] & 0x80) && (~m_KeyboardStateLast[(std::uint32_t)keycode] & 0x80)) ? true : false;
	}

	bool InputManager::IsKeyUp(KeyCode keycode) const
	{
		return (m_KeyboardState[(std::uint32_t)keycode] & 0x80) ? false	: true;
	}

	bool InputManager::IsKeyDown(KeyCode keycode) const
	{
		return (m_KeyboardState[(std::uint32_t)keycode] & 0x80) ? true : false;
	}

	bool InputManager::Update()
	{
		if (Engine::GetGameWindow()->isActive())
		{
			// Read the current state of the keyboard.
			if (!ReadKeyboard())
				return false;

			// Read the current state of the mouse.
			if (!ReadMouse())
				return false;


			m_MouseLastState = m_MouseCurrState;
		}

		return true;
	}


	bool InputManager::ReadKeyboard()
	{
		if (m_InputKeyboard == nullptr)
			return false;


		HRESULT result;

		for (std::int32_t i = 0; i < 256; i++)
			m_KeyboardStateLast[i] = m_KeyboardState[i];

		// Read the keyboard device.
		result = m_InputKeyboard->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)&m_KeyboardState);
		if (FAILED(result))
		{
			// If the keyboard lost focus or was not acquired then try to get control back.
			if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
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

		HRESULT result;
		// Read the mouse device.
		result = m_InputMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_MouseCurrState);
		if (FAILED(result))
		{
			// If the mouse lost focus or was not acquired then try to get control back.
			if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
				m_InputMouse->Acquire();
			else
				return false;

		}

		return true;
	}

	bool InputManager::Initialize()
	{
		HRESULT result = S_OK;

		Logger::Log("Input::Initialize");


		result = DirectInput8Create(Engine::GetGameWindow()->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, NULL);


		if (FAILED(result))
		{
			Logger::Error("Failed to create DirectInput8");
			return false;
		}

		//Create a keyboard device
		result = m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_InputKeyboard, NULL);
		if (FAILED(result))
		{
			Logger::Error("Failed to create keyboard device");
			return false;
		}


		// Set the data format.  
		result = m_InputKeyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(result))
			return false;


	if(Engine::GetGameWindow()->isChild() == false)
		result = m_InputKeyboard->SetCooperativeLevel(Engine::GetGameWindow()->GetHWND(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	else
		result = m_InputKeyboard->SetCooperativeLevel(NULL, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

		if (FAILED(result))
		{
			Logger::Error("m_InputKeyboard -> SetCooperativeLevel");
			return false;
		}


		result = m_InputKeyboard->Acquire();
		if (FAILED(result))
			return false;



		//Create a mouse device
		result = m_DirectInput->CreateDevice(GUID_SysMouse, &m_InputMouse, NULL);
		if (FAILED(result))
		{
			Logger::Error("Failed to create mouse device");
			return false;
		}


		// Set the data format for the mouse using the pre-defined mouse data format.
		result = m_InputMouse->SetDataFormat(&c_dfDIMouse);
		if (FAILED(result))
			return false;

		// Set the cooperative level of the mouse to share with other programs.
		if (Engine::GetGameWindow()->isChild() == false)
			result = m_InputMouse->SetCooperativeLevel(Engine::GetGameWindow()->GetHWND(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
		else
			result = m_InputMouse->SetCooperativeLevel(NULL, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

		if (FAILED(result))
		{
			Logger::Error("m_InputMouse -> SetCooperativeLevel");
			return false;
		}


		result = m_InputMouse->Acquire();
		if (FAILED(result))
			return false;

		return true;
	}
}