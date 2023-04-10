#pragma once
#include "Base/Main/Common.h"
#include "Math/Color.h"

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/imgui_impl_sdl.h"
#include "Libs/imgui/imgui_impl_dx11.h"

#include "atlbase.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#ifdef _DEBUG
//This macro enable debug layer, disable shader optimization and enable shader debug mode
#define S2DE_DEBUG_RENDER_MODE 
#endif

//Name of default font in resources  
#define S2DE_DEFAULT_FONT_NAME "default"

namespace S2DE
{
	namespace Editor
	{
		class EditorCenterCursor;
	}
}

namespace S2DE::Render
{
	enum class RenderFillMode : std::int32_t
	{
		Solid = D3D11_FILL_MODE::D3D11_FILL_SOLID,
		Wireframe = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME,
	};

	static D3D11_RASTERIZER_DESC defaultRasterDesc =
	{
		/* FillMode 				*/	static_cast<D3D11_FILL_MODE>(RenderFillMode::Solid),
		/* CullMode 				*/	D3D11_CULL_BACK,
		/* FrontCounterClockwise  */	false,
		/* DepthBias 				*/	0,
		/* DepthBiasClamp 		*/		0.0f,
		/* SlopeScaledDepthBias 	*/	0.0f,
		/* DepthClipEnable 			*/	true,
		/* ScissorEnable 			*/	false,
		/* MultisampleEnable 		*/	false,
		/* AntialiasedLineEnable	*/	true,
	};

	class S2DE_API Renderer
	{
	public:
		Renderer();
		~Renderer();

		// Initialize all renderer components.
		bool								Create();
		// Recreate: depthStencil, targetView, etc. Resize buffers and update viewport.
		bool								Reset();
		// Destroy all renderer components
		void								Destroy();
		// Update render and presents a rendered image
		void								Render();
		// Set new fill mode
		void								SwitchFillMode(RenderFillMode mode);
		// Set new back color
		void								SetBackColor(Math::Color<float> color);
		// Set disabled depth state 
		void								TurnZBufferOff();
		// Set enabled depth state 
		void								TurnZBufferOn();
		// Set enabled blend state 
		void								TurnOnAlphaBlending();
		// Set disabled blend state 
		void								TurnOffAlphaBlending();
		// Toggle ImGui Windows visible (Works only for windows in storage)
		void								ToggleImGuiWindowsVisible();
		// Toggle ImGui Demo window
		void								ToggleImGuiDemoWindowVisible();

		void								SetVsync(bool vsync);
		// Set rasterize state by name
		void								SetRasterizerState(std::string name = "default");
		// Set rasterize state by rasterizer state pointer
		void								SetRasterizerState(ID3D11RasterizerState* raster);

		// Get rasterize state by name from rasterizerVariants storage
		[[nodiscard]] inline ID3D11RasterizerState*		GetRasterizerState(std::string name);

		[[nodiscard]] inline ID3D11Device* GetDevice();
		[[nodiscard]] inline ID3D11DeviceContext* GetContext();
		[[nodiscard]] inline IDXGISwapChain* GetSwapChain();
		[[nodiscard]] inline ID3D11RenderTargetView* GetRenderTargetView();
		[[nodiscard]] inline D3D11_VIEWPORT				GetViewport() const;
		[[nodiscard]] inline ID3D11Texture2D* GetDepthStencilBuffer();
		[[nodiscard]] inline ID3D11DepthStencilView* GetDepthStencilView();
		[[nodiscard]] inline ID3D11ShaderResourceView* GetFramebufferShaderResource() const;
		[[nodiscard]] inline ID3D11Texture2D* GetFramebufferTextureData() const;
		[[nodiscard]] inline bool GetVsync() const;
		
		void								DrawIndexed(std::uint64_t indexCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0, D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		void								Draw(std::uint64_t vertexCount, std::uint32_t startVertexLocation = 0, D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Update viewport 
		void								UpdateViewport();

	private:
		// Initialize ImGui Library
		bool								InitImGui();
		// Update ImGui windows saved in storage
		void								UpdateImGuiWindows();
		// Render ImGui windows saved in storage
		void								RenderImGui();
		// Load custom theme
		void								LoadCustomImguiTheme();
		// Destroy ImGui Library
		void								DestroyImGui();
		// Create device with correct driver type with swap chain 
		bool								CreateDeviceAndSwapChain();
		// Create DepthStencil and RenderTarget
		bool								CreateDepthStencil();
		// Create RenderTarget and frame buffer
		bool								CreateRenderTarget();
		// Clear 
		void								Clear();
		// Presents a rendered image to the user.
		void								End();
		// Create rasterizer
		bool								CreateRasterizerState(D3D11_RASTERIZER_DESC desc = defaultRasterDesc, std::string name = "default");
		// Create blend state
		bool								CreateBlendState();
		// Create framebuffer texture by back buffer
		bool								CreateFramebufferTexture(ID3D11Texture2D* sw_buff);
		// Update framebuffer
		void								UpdateFramebufferShaderResource();
		// Create debug layer if we are in debug build and added special macro S2DE_DEBUG_RENDER_MODE
		void								CreateDebugLayer();
		// Capture messages from debug layer and print it to logger
		bool								CaptureMessages();
		// Create engine window and editor ui if it's we are have editor flag
		void								CreateEngineWindowsAndEditorUI();

		D3D11_VIEWPORT				m_viewport;
		ID3D11Texture2D*			m_frameBufferData;
		ID3D11ShaderResourceView*	m_frameBufferShaderResourceView;
		ID3D11RenderTargetView*		m_frameRenderTarget;


		IDXGISwapChain*				m_swapChain;
		ID3D11Device*				m_device;
		ID3D11DeviceContext*		m_context;
		ID3D11RenderTargetView*		m_targetView;
		ID3D11Texture2D*			m_backBuffer;
		ID3D11Texture2D*			m_depthStencilBuffer;
		ID3D11DepthStencilView*		m_depthStencilView;
		ID3D11DepthStencilState*	m_depthStateEnabled;
		ID3D11DepthStencilState*	m_depthStateDisabled;

		bool						m_vsync;
		std::uint32_t				m_deviceFlags;
		Math::Color<float>			m_clearColor;
		RenderFillMode				m_fillMode;

		ID3D11Debug*				m_d3dDebug;
		ID3D11InfoQueue*			m_d3dInfoQueue;
	
		std::vector<std::pair<std::string, CComPtr<ID3D11RasterizerState>>> m_rasterizerVariants;

		ID3D11BlendState*			m_blendStateOn; // TODO: Many modes, and use not like this
		ID3D11BlendState*			m_blendStateOff; 

		bool						m_showImguiWindows;
		bool						m_showImguiDemoWindow;

		class Editor::EditorCenterCursor*				m_editorCenterCursor;
		class ImGui_Window*								m_editorToolStrip;
		std::vector<std::pair<std::string, class ImGui_Window*>>	m_windowsStorage;

	public:  
		template<typename T>
		T GetImGui_Window(std::string name) const
		{
			std::vector<std::pair<std::string, ImGui_Window*>>::const_iterator it = std::find_if(m_windowsStorage.begin(),
				m_windowsStorage.end(), [&name](std::pair<std::string, ImGui_Window*> const& elem) {
					return elem.first == name;
				});
			
			if (it != m_windowsStorage.end())
				return reinterpret_cast<T>(it->second);

			return nullptr;
		}

		ImGui_Window*				AddImGuiWindow(std::string name, ImGui_Window* wnd, bool visible = false);
		void						DeleteImGuiWindow(std::string name);



		void						DebugDrawLineCross(DirectX::SimpleMath::Vector3 pos,
													DirectX::SimpleMath::Vector3 rot,
													DirectX::SimpleMath::Vector3 scale,
													DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1, 1, 1, 1));

		void						DebugDrawLine(DirectX::SimpleMath::Vector3 begin,
												  DirectX::SimpleMath::Vector3 end,
												  DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1, 1, 1, 1));

		void						DebugDrawCube(DirectX::SimpleMath::Vector3 pos, 
												DirectX::SimpleMath::Vector3 rot = DirectX::SimpleMath::Vector3(0,0,0),
												DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(1, 1, 1),
												DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1,1,1,1));

		void						DebugDrawRing(DirectX::SimpleMath::Vector3 pos, 			
												  DirectX::SimpleMath::Vector3 majorAxis,
												  DirectX::SimpleMath::Vector3 minorAxis,
												  DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1, 1, 1, 1));

		void						DebugDrawSphere(DirectX::SimpleMath::Vector3 pos,
													float radius,
													DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1, 1, 1, 1));

	};											  
}												  