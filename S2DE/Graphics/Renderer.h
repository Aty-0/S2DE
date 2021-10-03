#pragma once
#include "Base/Main/Common.h"
#include "Math/Color.h"
#include "Math/Vector.h"

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/imgui_impl_win32.h"
#include "Libs/imgui/imgui_impl_dx11.h"

//ImGui WndProcHandler
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//Name of default font for ui_basic_text and imgui  
#define S2DE_DEFAULT_FONT_NAME "default"


namespace S2DE
{
	enum class RenderFillMode : std::int32_t
	{
		Solid = D3D11_FILL_MODE::D3D11_FILL_SOLID,
		Wireframe = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME,
	};

	class S2DE_API Renderer
	{
	public:
		Renderer();
		~Renderer();

		bool						Create();
		bool						Reset();
		void						Destroy();
		void						Render();

		inline ID3D11Device*		GetDevice() const { return m_device; }
		inline ID3D11DeviceContext* GetContext() const { return m_deviceContext; }
		inline IDXGISwapChain*		GetSwapChain() const { return m_swapChain; }
		inline bool					GetVsync() const { return m_vsync; }
		inline D3D11_VIEWPORT		GetViewport() const { return m_viewport; }

		void						SwitchFillMode(RenderFillMode mode);

	private:					 
		bool						InitImGui();
		void						UpdateImGuiWindows();
		void						RenderImGui();
		void						LoadCustomImguiTheme();
		void						DestroyImGui();
		bool						GetDisplay(std::uint32_t& mode_numerator, std::uint32_t& mode_denominator);
		bool						CreateDeviceAndSwapChain();
		bool						CreateDepthStencil();
		void						UpdateViewport();
		void						Clear();
		void						End();
		bool						CreateRasterizer();


		IDXGISwapChain*				m_swapChain;
		ID3D11Device*				m_device;
		ID3D11DeviceContext*		m_deviceContext;
		ID3D11RenderTargetView*		m_renderTargetView;
		ID3D11RasterizerState*		m_rasterState;
		VideoCardDesc				m_videocard_desc;

		//Depth strencil
		ID3D11Texture2D*			m_depthStencilBuffer;
		ID3D11DepthStencilState*	m_depthStencilState;
		ID3D11DepthStencilView*		m_depthStencilView;

		bool						m_vsync;
		D3D_FEATURE_LEVEL			m_feature_level;
		std::uint32_t				m_mode_numerator;
		std::uint32_t				m_mode_denominator;

		D3D11_VIEWPORT				m_viewport;

		std::uint32_t				m_device_flag;

		//TODO
		//Own class
		// 0.3f, 0.1f, 0.4f, 1.0f 
		float						m_clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		std::vector<std::pair<std::string, class ImGui_Window*>>	m_windows_storage;

		RenderFillMode				m_render_fill_mode;
	public:  
		inline class ImGui_Window*	GetImGui_Window(std::string name) const;
		void						AddImGuiWindow(std::string name, ImGui_Window* wnd);
		void						DeleteImGuiWindow(std::string name);
	};
}