#pragma once
#include "Render/RenderCommon.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#ifdef _DEBUG
//This macro enable debug layer, disable shader optimization and enable shader debug mode
#define S2DE_DEBUG_RENDER_MODE 
#endif

namespace S2DE
{
	namespace Editor
	{
		class EditorCenterCursor;
	}
}

namespace S2DE::Render
{
	class S2DE_API Renderer : public Core::Utils::Singleton<Renderer>
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
		void								SetRasterizerState(Api::RasterizerMode mode);
		
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
		bool								CreateRasterizerStates();
		// Create blend state
		bool								CreateBlendState();
		// Create framebuffer texture by back buffer
		bool								CreateFramebufferTexture(ID3D11Texture2D* sw_buff);
		// Update framebuffer
		void								UpdateFramebufferShaderResource();
		// Create debug layer if we are in debug build and added special macro S2DE_DEBUG_RENDER_MODE
		bool								CreateDebugLayer();
		// Capture messages from debug layer and print it to logger
		bool								CaptureMessages();
		// Create engine window and editor ui if it's we are have editor flag
		void								CreateEngineWindowsAndEditorUI();


		struct Framebuffer
		{
			ID3D11Texture2D*			BufferData;
			ID3D11ShaderResourceView*	BufferSRV;
			ID3D11RenderTargetView*		RenderTarget;
		};

		Framebuffer	m_framebuffer;

		struct Base
		{
			IDXGISwapChain*				SwapChain;
			ID3D11Device*				Device;
			ID3D11DeviceContext*		Context;
			ID3D11RenderTargetView*		RenderTarget;
			D3D11_VIEWPORT				Viewport;
			
			std::uint32_t				DeviceFlags;

			std::map<Api::RasterizerMode, CComPtr<ID3D11RasterizerState>> Rasterizer;
		};

		Base m_base;

		struct ZBuffer
		{
			ID3D11Texture2D*			BackBuffer;
			ID3D11Texture2D*			DepthStencilBuffer;
			ID3D11DepthStencilView*		DepthStencilView;
			ID3D11DepthStencilState*	DepthState[2]; // 0 - Enabled 1 - Disabled
		};

		ZBuffer m_zBuffer;

		struct Params
		{
			Math::Color<float>			ClearColor;
			Api::FillMode				FillMode;
			bool						Vsync;
			bool						ShowImguiWindows;
			bool						ShowImguiDemoWindow;
		};

		Params m_params;

		struct Debug
		{
			ID3D11Debug*				DebugCore;
			ID3D11InfoQueue*			InfoQueue;	
		};

		Debug m_debug;

		struct Blend
		{
			// TODO: Hash by diffrent modes
			//		 Currently we are have some custom blend mode and disabled variant 

			ID3D11BlendState*			BlendState[2]; 
		};

		Blend m_blend;


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



		void						DebugDrawLineCross(Math::float3 pos, Math::float3 rot, Math::float3 scale,
													   DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1, 1, 1, 1));

		void						DebugDrawLine(Math::float3 begin, Math::float3 end,
												  DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1, 1, 1, 1));

		void						DebugDrawCube(Math::float3 pos, Math::float3 rot = Math::float3(0,0,0), Math::float3 scale = Math::float3(1, 1, 1),
												  DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1,1,1,1));

		void						DebugDrawRing(Math::float3 pos, Math::float3 majorAxis, Math::float3 minorAxis,
												  DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1, 1, 1, 1));

		void						DebugDrawSphere(Math::float3 pos, float radius,
													DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1, 1, 1, 1));

	};											  
}												  