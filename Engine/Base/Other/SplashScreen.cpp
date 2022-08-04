#include "SplashScreen.h"
#include "resource.h"
#include "Base/Main/BuildDate.h"
#include "Base/Engine.h"

#include <winuser.h>

//#define S2DE_DRAW_EDITOR_PROJECT_NAME_LABEL
//#define S2DE_DRAW_EDITOR_MODE_LABEL


namespace S2DE::Core::Other
{
	SplashScreen* SplashScreen::m_instanceWindow;
	std::string	SplashScreen::m_loadState;
	std::string	SplashScreen::m_projectName;

	SplashScreen::SplashScreen()
	{
		m_instanceWindow = this;

		m_text_build_str = "Build:" + std::string(S2DE_BUILD_DATE);
	}


	SplashScreen::~SplashScreen()
	{
		ReleaseDC(m_hwnd, m_hdcStatic);
		ReleaseDC(m_hwnd, m_hdc_bp);
		ReleaseDC(m_hwnd, m_hdcMem_bp);
		DeleteObject(m_hwnd);
		DeleteObject(m_hinstance);
		DeleteObject(m_hBitmap);
		DeleteObject(m_hOldBitmap);
		DeleteObject(m_hfont_LoadState);
		DeleteObject(m_hfont_Build);
		DeleteObject(m_hfont_Ed);
		DeleteObject(m_hfont_Name_Ed);
		FreeLibrary(m_res_hInst);
		m_text_build_str.clear();
		m_loadState.clear();
		m_projectName.clear();
		m_text_ed_str.clear();
	}

	LRESULT CALLBACK SplashScreen::InstanceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return SplashScreen::GetInstanceWindow()->WndProc(hWnd, msg, wParam, lParam);
	}

	void SplashScreen::RedrawWindowElements()
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		RedrawWindow(m_hwnd, &rc, 0, RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_ALLCHILDREN | RDW_INVALIDATE);
	}

	void SplashScreen::SetProjectName(std::string name)
	{
#ifdef S2DE_DRAW_EDITOR_PROJECT_NAME_LABEL
		isStringEmpty(name) ? m_projectName = "No name" : m_projectName = name;
#endif
	}

	void SplashScreen::SetLoadState(std::string state, SplashScreen* splashScreen)
	{
		if (splashScreen)
		{
			m_loadState = state;
			splashScreen->RedrawWindowElements();
		}
	}

	void SplashScreen::Close()
	{
		UnregisterClass(m_splashWindowClass.lpszClassName, m_hinstance);
		DestroyWindow(m_hwnd);
	}

	bool SplashScreen::ShowSplashScreen(HINSTANCE hinstance)
	{
		m_res_hInst = LoadLibrary(S2DE_NAME_DLL);
		if (m_res_hInst == NULL)
			return false;

		m_hBitmap = (HBITMAP)LoadBitmap(m_res_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		if (m_hBitmap == NULL)
			return false;

		GetObject(m_hBitmap, sizeof(BITMAP), &m_bitmap);

		std::int32_t SplashScreenX = (GetSystemMetrics(SM_CXSCREEN)  / 2) - m_bitmap.bmWidth / 2;
		std::int32_t SplashScreenY = (GetSystemMetrics(SM_CYSCREEN) / 2) - m_bitmap.bmHeight / 2;
		std::int32_t SplashScreenMenuHeight = GetSystemMetrics(SM_CYMENU);
		std::int32_t SplashScreenFrameWidth = GetSystemMetrics(SM_CXSIZEFRAME);
		std::int32_t SplashScreenFrameHeight = GetSystemMetrics(SM_CYSIZEFRAME);
		std::int32_t SplashScreenWindowWidth = m_bitmap.bmWidth + SplashScreenFrameWidth * 2;
		std::int32_t SplashScreenWindowHeight = m_bitmap.bmHeight + SplashScreenFrameHeight * 2 + SplashScreenMenuHeight;
	
		ZeroMemory(&m_splashWindowClass, sizeof(WNDCLASSEX));
		m_splashWindowClass.cbSize = sizeof(WNDCLASSEX);
		m_splashWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		m_splashWindowClass.lpfnWndProc = InstanceWndProc;
		m_splashWindowClass.hInstance = hinstance;
		m_splashWindowClass.hbrBackground = NULL;
		m_splashWindowClass.lpszMenuName = NULL;
		m_splashWindowClass.lpszClassName = m_className;

		RegisterClassEx(&m_splashWindowClass);

		m_hwnd = CreateWindowEx(NULL, m_className, m_name, WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE,
			SplashScreenX, SplashScreenY, SplashScreenWindowWidth, SplashScreenWindowHeight, NULL, NULL, m_splashWindowClass.hInstance, NULL);

		if (!ShowWindow(m_hwnd, SW_SHOW))
			return false;

		HRGN hrgn = CreateRectRgn(0, 0, m_bitmap.bmWidth, m_bitmap.bmHeight);
		SetWindowRgn(m_hwnd, hrgn, TRUE);
		SetForegroundWindow(m_hwnd);
		SetFocus(m_hwnd);
		UpdateWindow(m_hwnd);

		return true;
	}

	LRESULT CALLBACK SplashScreen::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_CREATE:
			m_hdc_bp = GetDC(hWnd);
			m_hdcMem_bp = CreateCompatibleDC(m_hdc_bp);
			m_hOldBitmap = (HBITMAP)SelectObject(m_hdcMem_bp, m_hBitmap);

			ReleaseDC(hWnd, m_hdc_bp);

			m_hfont_LoadState = CreateFont(-15, 0, 0, 0, FW_LIGHT, 0,
				0, 0, 0, 0, 0, 0, 0, "Arial");

			m_hfont_Build = CreateFont(-15, 0, 0, 0, FW_LIGHT, 0,
				0, 0, 0, 0, 0, 0, 0, "Arial");

			if (Engine::isEditor())
			{
#ifdef S2DE_DRAW_EDITOR_PROJECT_NAME_LABEL
				m_hfont_Name_Ed = CreateFont(-20, 0, 0, 0, FW_LIGHT, 0,
					0, 0, 0, 0, 0, 0, 0, "Arial");
#endif

#ifdef S2DE_DRAW_EDITOR_MODE_LABEL
				m_hfont_Ed = CreateFont(-30, 0, 0, 0, FW_LIGHT, 0,
					0, 0, 0, 0, 0, 0, 0, "Arial");

				m_text_ed_str = "Editor";
#endif
			}

			break;
		case WM_PAINT:
			PAINTSTRUCT ps;
			HDC phdc;

			phdc = BeginPaint(hWnd, &ps);

			SetStretchBltMode(m_hdc_bp, STRETCH_HALFTONE);
			StretchBlt(m_hdc_bp, 0, 0, m_bitmap.bmWidth, m_bitmap.bmHeight,
				m_hdcMem_bp, 0, 0, m_bitmap.bmWidth, m_bitmap.bmHeight, SRCCOPY);

		
			SetTextColor(phdc, RGB(255, 255, 255));
			SetBkMode(phdc, TRANSPARENT);

			if (Engine::isEditor())
			{				
#ifdef S2DE_DRAW_EDITOR_PROJECT_NAME_LABEL	
				SelectObject(phdc, m_hfont_Ed);
				TextOut(phdc, 5, 0, m_projectName.c_str(), (std::int32_t)m_projectName.length());
#endif

#ifdef S2DE_DRAW_EDITOR_MODE_LABEL
				SelectObject(phdc, m_hfont_Name_Ed);
				TextOut(phdc, 5, 50, m_text_ed_str.c_str(), (std::int32_t)m_text_ed_str.length());
#endif
			}


			SelectObject(phdc, m_hfont_Build);
			TextOut(phdc, 13, m_bitmap.bmHeight - 25, m_text_build_str.c_str(), (std::int32_t)m_text_build_str.length());

			SelectObject(phdc, m_hfont_LoadState);
			TextOut(phdc, m_bitmap.bmWidth / 2 - 50, m_bitmap.bmHeight - 25, m_loadState.c_str(), (std::int32_t)m_loadState.length());
	

			EndPaint(hWnd, &ps);
			break;
		}


		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}