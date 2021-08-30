#include "SplashScreen.h"
#include "resource.h"
#include "Base/Main/BuildDate.h"
#include "Base/Engine.h"

#include <winuser.h>

namespace S2DE
{
	SplashScreen* SplashScreen::m_instanceWindow;

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
		m_text_load_state_str.clear();
		m_text_project_name_str.clear();
		m_text_ed_str.clear();
	}

	LRESULT CALLBACK SplashScreen::InstanceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return SplashScreen::GetInstanceWindow()->WndProc(hWnd, msg, wParam, lParam);
	}

	void SplashScreen::SetProjectName(std::string name)
	{
		isStringEmpty(name) ? m_text_project_name_str = "S2DE" : m_text_project_name_str = name ;

		RECT rc;
		GetClientRect(m_hwnd, &rc);
		RedrawWindow(m_hwnd, &rc, 0, RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_ALLCHILDREN | RDW_INVALIDATE);

	}

	void SplashScreen::SetLoadState(std::string state)
	{
		m_text_load_state_str = state;

		RECT rc;
		GetClientRect(m_hwnd, &rc);
		RedrawWindow(m_hwnd, &rc, 0, RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_ALLCHILDREN | RDW_INVALIDATE);
	}

	void SplashScreen::Close()
	{
		UnregisterClass(m_splashWindowClass.lpszClassName, m_hinstance);
		DestroyWindow(m_hwnd);
	}

	bool SplashScreen::ShowSplashScreen(HINSTANCE hinstance)
	{
		std::int32_t SplashScreenX = (GetSystemMetrics(SM_CXSCREEN)  / 2) - 200;
		std::int32_t SplashScreenY = (GetSystemMetrics(SM_CYSCREEN) / 2) - 100;
		std::int32_t SplashScreenMenuHeight = GetSystemMetrics(SM_CYMENU);
		std::int32_t SplashScreenFrameWidth = GetSystemMetrics(SM_CXSIZEFRAME);
		std::int32_t SplashScreenFrameHeight = GetSystemMetrics(SM_CYSIZEFRAME);
		std::int32_t SplashScreenWindowWidth = m_defaultPicWidth + SplashScreenFrameWidth * 2;
		std::int32_t SplashScreenWindowHeight = m_defaultPicHeight + SplashScreenFrameHeight * 2 + SplashScreenMenuHeight;
	
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

		UpdateWindow(m_hwnd);

		HRGN hrgn = CreateRectRgn(0, 0, m_defaultPicWidth, m_defaultPicHeight);
		SetWindowRgn(m_hwnd, hrgn, TRUE);


		return true;
	}

	LRESULT CALLBACK SplashScreen::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_CREATE:
			m_res_hInst = LoadLibrary("S2DE_DX11.dll");
			m_hBitmap = (HBITMAP)LoadBitmap(m_res_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			GetObject(m_hBitmap, sizeof(BITMAP), &m_bitmap);

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
				m_hfont_Name_Ed = CreateFont(-25, 0, 0, 0, FW_LIGHT, 0,
					0, 0, 0, 0, 0, 0, 0, "Arial");

				m_hfont_Ed = CreateFont(-45, 0, 0, 0, FW_LIGHT, 0,
					0, 0, 0, 0, 0, 0, 0, "Arial");

				m_text_ed_str = "Editor";
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
				SelectObject(phdc, m_hfont_Ed);
				TextOut(phdc, 5, 0, m_text_project_name_str.c_str(), (std::int32_t)m_text_project_name_str.length());
				SelectObject(phdc, m_hfont_Name_Ed);
				TextOut(phdc, 5, 50, m_text_ed_str.c_str(), (std::int32_t)m_text_ed_str.length());
			}


			SelectObject(phdc, m_hfont_Build);
			TextOut(phdc, 13, m_defaultPicHeight - 30, m_text_build_str.c_str(), (std::int32_t)m_text_build_str.length());

			SelectObject(phdc, m_hfont_LoadState);
			TextOut(phdc, m_defaultPicWidth / 2 - 50, m_defaultPicHeight - 30, m_text_load_state_str.c_str(), (std::int32_t)m_text_load_state_str.length());
	

			EndPaint(hWnd, &ps);
			break;
		}


		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}