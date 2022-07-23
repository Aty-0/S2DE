#include "GameWindow.h"
#include "Base/Engine.h"
#include "Base/ApplicationHandle.h"
#include "Base/Main/BuildDate.h"
#include "Render/Renderer.h"

#define S2DE_WINDOW_CLASS_NAME "S2DE_WND_CLASS_NAME"

//Styles
#define S2DE_DEFAULT_WINDOW_STYLE WS_TILEDWINDOW //WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX
#define S2DE_FULLSCREEN_WINDOW_STYLE WS_EX_TOPMOST | WS_POPUP
#define S2DE_CHILD_WINDOW_STYLE WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP

namespace S2DE::Core
{
	GameWindow* GameWindow::m_InstanceWindow;

	GameWindow::GameWindow() :
		m_Fullscreen(false),
		m_Height(0),
		m_Width(0),
		m_HWND(NULL),
		m_Instance(NULL),
		m_isChild(false),
		m_isClosing(false),
		m_Left(0),
		m_Top(0),
		m_Name(),
		m_PreviousHeight(0),
		m_PreviousWidth(0),
		m_ShowCursor(false)
	{
		m_InstanceWindow = this;
	}


	GameWindow::~GameWindow()
	{
		Delete(m_InstanceWindow);
		m_Top = 0;
		m_Left = 0;
		m_Fullscreen = false;
		m_Instance = NULL;
		m_HWND = NULL;
		m_ShowCursor = false;
		m_Width = 0;
		m_Height = 0;
	}

	bool GameWindow::AttachChildWindow(HINSTANCE hInstance, std::uint32_t ClientRectangleX, std::uint32_t ClientRectangleY, std::uint32_t Width, std::uint32_t Height, HWND Handle)
	{
		Logger::Log("Try to attach child window...");

		m_isChild = true;
		m_Fullscreen = false;
		m_Top = ClientRectangleX;
		m_Left = ClientRectangleY;
		m_Instance = hInstance;
		m_Width = Width;
		m_Height = Height;


		ZeroMemory(&m_WindowClass, sizeof(WNDCLASSEX));
		m_WindowClass.cbSize = sizeof(WNDCLASSEX);
		m_WindowClass.style = CS_HREDRAW | CS_VREDRAW;
		m_WindowClass.lpfnWndProc = InstanceWndProc;
		m_WindowClass.hInstance = m_Instance;
		m_WindowClass.hbrBackground = NULL;
		m_WindowClass.lpszMenuName = NULL;
		m_WindowClass.lpszClassName = S2DE_WINDOW_CLASS_NAME;


		RegisterClassEx(&m_WindowClass);

		m_HWND = CreateWindowEx(NULL, S2DE_WINDOW_CLASS_NAME, NULL, S2DE_CHILD_WINDOW_STYLE,
			ClientRectangleX, ClientRectangleY, Width, Height, Handle, NULL, m_WindowClass.hInstance, NULL);

		if (m_HWND == NULL)
		{
			S2DE_FATAL_ERROR("Can't create child window!");
			return false;
		}



		ShowWindow(m_HWND, SW_SHOW);
		UpdateWindow(m_HWND);

		return true;
	}

	RECT GameWindow::GetClientRes() const
	{
		RECT rc;
		GetWindowRect(GetDesktopWindow(), &rc);
		return rc;
	}

	void GameWindow::SetClientScreenRes()
	{
		Logger::Log("Set client screen resolution to game window...");
		m_Width	 = GetClientRes().right;
		m_Height = GetClientRes().bottom;

		MoveWindow(m_HWND, m_Top, m_Left, m_Width, m_Height, TRUE);
	}

	bool GameWindow::Create(HINSTANCE hInstance, std::string name, std::uint32_t w, std::uint32_t h, std::int32_t top, std::int32_t left, bool Show_Cursor, bool Fullscreen)
	{
		Logger::Log("Create game window...");

		//Get desktop resolution if fullscreen mode on
		m_Width = Fullscreen == true ? GetClientRes().right : w;
		m_Height = Fullscreen == true ? GetClientRes().bottom : h;
		m_Fullscreen = Fullscreen;
		m_Top = top;
		m_Left = left;
		m_ShowCursor = Show_Cursor;
		m_Instance = hInstance;

		std::string str;

#ifdef NDEBUG
		str = Core::Utils::isStringEmpty(name) ? "S2DE " + std::string(S2DE_BUILD_DATE) : name;
		
		if (Engine::isEditor())
			str.append(" Editor");

#else
		str = "S2DE " + std::string(S2DE_BUILD_DATE);

		if (Engine::isEditor())
			str.append(" Editor");

		if (!Core::Utils::isStringEmpty(name))
			str.append(std::string(" (") + name + ")");

		str.append(" Debug");
#endif
		m_WindowClass = { };

		//Setup window class
		m_WindowClass.cbSize = sizeof(WNDCLASSEXW);
		m_WindowClass.style = CS_HREDRAW | CS_VREDRAW;
		m_WindowClass.lpfnWndProc = InstanceWndProc;
		m_WindowClass.hInstance = m_Instance;
		m_WindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		m_WindowClass.hIconSm = m_WindowClass.hIcon;
		m_WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
		m_WindowClass.hbrBackground = NULL;
		m_WindowClass.lpszMenuName = NULL;
		m_WindowClass.lpszClassName = S2DE_WINDOW_CLASS_NAME;
		m_WindowClass.cbClsExtra = 0;
		m_WindowClass.cbWndExtra = 0;
		RegisterClassEx(&m_WindowClass);


		RECT rc = { 0, 0, (std::int32_t)m_Width, (std::int32_t)m_Height };
		AdjustWindowRect(&rc, Fullscreen ? S2DE_FULLSCREEN_WINDOW_STYLE : S2DE_DEFAULT_WINDOW_STYLE, false);

		m_HWND = CreateWindowExA(NULL, S2DE_WINDOW_CLASS_NAME, str.c_str(), Fullscreen ? S2DE_FULLSCREEN_WINDOW_STYLE : S2DE_DEFAULT_WINDOW_STYLE,
			m_Left, m_Top, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, m_WindowClass.hInstance, NULL);

		if (m_HWND == NULL)
		{
			S2DE_FATAL_ERROR("Can't create game window");
			return false;
		}

		ShowCursor(m_ShowCursor);
		SetForegroundWindow(m_HWND);
		ShowWindow(m_HWND, SW_HIDE);

		RECT wnd_rect;
		GetWindowRect(m_HWND, &wnd_rect);

		return true;
	}

	void GameWindow::Restore()
	{
		ShowWindow(m_HWND, SW_SHOW);
	}

	void GameWindow::SetMouseVisible(bool visible)
	{
		if (m_ShowCursor != visible) // Don't call any function if we are have same parameter state 
		{
			m_ShowCursor = visible;
			SetCursorPos(m_Width / 2, m_Height / 2);
			ShowCursor(m_ShowCursor);
			SetFocus(m_HWND);
		}
	}

	void GameWindow::SetWidht(std::uint32_t w)
	{
		m_Width = w;
		MoveWindow(m_HWND, m_Top, m_Left, m_Width, m_Height, TRUE);
	}

	void GameWindow::SetHeight(std::uint32_t h)
	{
		m_Height = h;
		MoveWindow(m_HWND, m_Top, m_Left, m_Width, m_Height, TRUE);
	}

	void GameWindow::SetTop(std::int32_t top)
	{
		m_Top = top;
		MoveWindow(m_HWND, m_Top, m_Left, m_Width, m_Height, TRUE);
	}

	void GameWindow::SetLeft(std::int32_t left)
	{
		m_Left = left;
		MoveWindow(m_HWND, m_Top, m_Left, m_Width, m_Height, TRUE);
	}

	void GameWindow::SetFullscreen(bool fullscreen)
	{
		m_Fullscreen = fullscreen;

		if (m_Fullscreen == true)
		{
			Logger::Log("Set fullscreen mode...");

			m_PreviousWidth = m_Width;
			m_PreviousHeight = m_Height;

			SetWindowLong(m_HWND, GWL_STYLE, S2DE_FULLSCREEN_WINDOW_STYLE);
			SetWindowLong(m_HWND, GWL_EXSTYLE, WS_EX_TOPMOST);

			GetWindowPlacement(m_HWND, &m_WindowPlacement);
			ShowWindow(m_HWND, SW_SHOWMAXIMIZED);

			m_Width = GetClientRes().right;
			m_Height = GetClientRes().bottom;
		}
		else
		{
			Logger::Log("Set window mode...");

			m_Width = m_PreviousWidth;
			m_Height = m_PreviousHeight;

			m_PreviousWidth = 0;
			m_PreviousHeight = 0;

			SetWindowLong(m_HWND, GWL_STYLE, S2DE_DEFAULT_WINDOW_STYLE);
			SetWindowLong(m_HWND, GWL_EXSTYLE, 0L);

			MoveWindow(m_HWND, m_Top, m_Left, m_Width, m_Height, TRUE);
			SetWindowPlacement(m_HWND, &m_WindowPlacement);
			ShowWindow(m_HWND, SW_SHOW);
		}


		Engine::GetRenderer()->Reset();
	}

	void GameWindow::Close()
	{
		if (m_HWND != NULL)
		{
			m_isClosing = true;
			UnregisterClass(S2DE_WINDOW_CLASS_NAME, m_Instance);
			DestroyWindow(m_HWND);
			m_HWND = NULL;
			m_Instance = NULL;

			Logger::Log("Game window closed");
		}
	}

	LRESULT CALLBACK GameWindow::InstanceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return GameWindow::GetInstanceWindow()->WndProc(hWnd, msg, wParam, lParam);
	}


	LRESULT CALLBACK GameWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return 0;

		switch (msg)
		{
		case WM_PAINT:
			OnPaint(hWnd);
			break;
		case WM_MOVING:
			OnMoving();
			break;
		case WM_SIZE:
			if (!m_isChild)
				OnSizeChanged();
			break;
		case WM_SIZING:
			OnSizing();
			break;
		case WM_CLOSE:
			OnClose();
			return 0;
		case WM_DESTROY:
			OnDestroy();
			break;
		}


		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	void GameWindow::OnPaint(HWND hWnd)
	{
		PAINTSTRUCT ps;

		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}

	void GameWindow::OnMoving()
	{
		RECT rc;
		GetWindowRect(m_HWND, &rc);

		m_Left	 = rc.left;
		m_Top	 = rc.top;
	}

	void GameWindow::OnSizing()
	{

	}

	void GameWindow::OnClose()
	{
		m_isClosing = true;
		Engine::Destroy();
	}

	void GameWindow::OnDestroy()
	{
		PostQuitMessage(0);
	}

	void GameWindow::OnSizeChanged()
	{
		if (m_Fullscreen == false)
		{
			RECT wnd_rect;
			GetWindowRect(m_HWND, &wnd_rect);

			m_Width =  wnd_rect.right -  wnd_rect.left;
			m_Height = wnd_rect.bottom - wnd_rect.top;

			if (Engine::GetRenderer() != nullptr)
			{
				Engine::GetRenderer()->Reset();
				Engine::GetApplicationHandle()->OnWindowResize();
			}
		}
	}

	bool GameWindow::ProcessMessage()
	{
		MSG msg;
		
		if (PeekMessage(&msg, m_HWND, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_NULL)
			{
				if (!IsWindow(m_HWND))
				{
					Close();
					return false;
				}
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return true;
	}
}