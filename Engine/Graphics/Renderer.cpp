#include "Renderer.h"

#include "Base/Engine.h"
#include "Base/DebugTools/VisualConsole.h"
#include "Base/ApplicationHandle.h"
#include "Base/GameWindow.h"

#include "Scene/SceneManager.h"

#include "Graphics/ImGui_Window.h"

#include "Base/DebugTools/Debug_Info.h"
#include "Base/DebugTools/Debug_ObjectInspector.h"

#include "Editor/EditorToolstrip.h"
#include "Editor/EditorObjectInspector.h"
#include "Editor/EditorRenderWindow.h"
#include "Editor/EditorCursor.h"
#include "Editor/EditorColorPicker.h"
#include "Editor/EditorCenterCursor.h"

#include <dxgidebug.h>

#define S2DE_IMGUI_NEW_FRAME()  ImGui_ImplDX11_NewFrame(); \
								ImGui_ImplWin32_NewFrame(); \
								ImGui::NewFrame(); \


using namespace S2DE::Core;
using namespace S2DE::Core::Other;
using namespace S2DE::Core::Debug;
using namespace S2DE::Math;
using namespace S2DE::Editor;

namespace S2DE::Render
{
	Renderer::Renderer() :	m_swapChain(nullptr),
							m_device(nullptr),
							m_deviceContext(nullptr),
							m_renderTargetView(nullptr),
							m_rasterState(nullptr),
							m_depthStencilBuffer(nullptr),
							m_depthStencilState(nullptr),
							m_depthStencilView(nullptr),
							m_blendState(nullptr),
							m_vsync(true),
							m_show_imgui_windows(true),
							m_show_imgui_demo_wnd(true),
							m_device_flag(0),
							m_render_fill_mode(RenderFillMode::Solid),
							m_clearColor(Color<float>(0.0f, 0.0f, 0.0f, 1.0f))

	{

	}


	Renderer::~Renderer()
	{

	}

	bool Renderer::Reset()
	{
#if 1
		Logger::Log("[Renderer] Device reset");

		//Remove previous render target
		m_deviceContext->OMSetRenderTargets(0, 0, 0);
		Release(m_renderTargetView);
		
		//Reset buffers
		S2DE_CHECK(m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0), "Render Error: Can't reset buffers");
		
		ID3D11Texture2D* pBuffer;
		m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
		S2DE_CHECK(m_device->CreateRenderTargetView(pBuffer, NULL, &m_renderTargetView), "Render Error: Can't recreate render target view");
		
		CreateFramebufferTexture(pBuffer);
		Release(pBuffer);
		
		m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);
		
		UpdateViewport();
#else

		S2DE_NO_IMPL();
#endif // 1
		return true;
	}

	void Renderer::LoadCustomImguiTheme()
	{
		//Rounded edges
		//ImGui::GetStyle().FrameRounding = 4.0f;
		//ImGui::GetStyle().GrabRounding = 4.0f;
		//ImGui::GetStyle().TabRounding = 4.0f;
		//ImGui::GetStyle().WindowRounding = 6.0f;

		ImGui::GetStyle().FrameRounding = 1.0f;
		ImGui::GetStyle().GrabRounding = 2.0f;
		ImGui::GetStyle().TabRounding = 2.0f;
		ImGui::GetStyle().WindowRounding = 0.0f;

		ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f);
		//ImGui::GetStyle().WindowMinSize = ImVec2(100.0f, 400.0f);
		
		ImVec4* colors = ImGui::GetStyle().Colors;

		//colors[ImGuiCol_Text] = ImVec4(1.0f, 0.3f, 0.3f, 0.8f);
		colors[ImGuiCol_Text] = ImVec4(1.0f, 0.89f, 1.0f, 0.8f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);

		colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.8f);
		
		colors[ImGuiCol_ChildBg] = ImVec4(0.04f, 0.04f, 0.04f, 0.6f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
			
		colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

		colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);

		colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.1f, 0.1f, 0.65f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.05f, 0.05f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);

		colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 0.5f, 0.5f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.7f, 0.56f, 0.56f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.5f, 0.5f, 1.00f);

		colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(1.0f, 0.5f, 0.5f, 1.00f);

		colors[ImGuiCol_Header] = ImVec4(0.14f, 0.14f, 0.14f, 0.55f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.34f, 0.24f, 0.24f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);

		colors[ImGuiCol_Separator] = ImVec4(0.7f, 0.7f, 0.7f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.1f, 0.1f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.1f, 0.1f, 1.00f);

		colors[ImGuiCol_ResizeGrip] = ImVec4(0.24f, 0.24f, 0.24f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.5f, 0.5f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.5f, 0.5f, 0.95f);

		colors[ImGuiCol_Tab] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.1f, 0.1f, 0.1f, 0.80f);
		colors[ImGuiCol_TabActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);

		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);

		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	bool Renderer::InitImGui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); 
		io.ConfigFlags = Engine::isEditor() ? ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NoMouseCursorChange 
			: ImGuiConfigFlags_NoMouseCursorChange;

		//Search custom font
		std::string path;
		if (Engine::GetResourceManager().GetFilePath(S2DE_DEFAULT_FONT_NAME, "Font", ".ttf", path))
		{
			Logger::Log("Added custom font for imgui...");
			io.Fonts->AddFontFromFileTTF(path.c_str(), 16);
		}

		LoadCustomImguiTheme();

		//Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(Engine::GetGameWindow()->GetHWND());
		ImGui_ImplDX11_Init(m_device, m_deviceContext);

		return true;
	}

	bool Renderer::CreateDeviceAndSwapChain()
	{
		Logger::Log("[Renderer] Create device...");

		D3D_FEATURE_LEVEL FeatureLevels[] = 
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
			D3D_DRIVER_TYPE_UNKNOWN,
		};

		//m_device_flag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;


#if defined(_DEBUG) && defined(S2DE_DEBUG_RENDER_MODE)
		Logger::Log("[Renderer] Render in debug mode!");
		m_device_flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		std::int32_t driver_types_size = ARRAYSIZE(driverTypes);

		for (std::uint32_t driverTypeIndex = 0; driverTypeIndex < driver_types_size; driverTypeIndex++)
		{
			HRESULT hr = S_OK;
			D3D_DRIVER_TYPE driverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDevice(nullptr, driverType, nullptr, m_device_flag, FeatureLevels, ARRAYSIZE(FeatureLevels), D3D11_SDK_VERSION, &m_device, nullptr, &m_deviceContext);
			if (hr == E_INVALIDARG)
				continue;


			if (SUCCEEDED(hr))
			{
				Logger::Log("[Renderer] Succeeded created device!");
				Logger::Log("[Renderer] Some info: driverTypeIndex -> %d feature level -> 0x%0X", driverTypeIndex, m_device->GetFeatureLevel());
				break;
			}
		}

		if (m_device == nullptr)
			S2DE_FATAL_ERROR("[Renderer] Can't create device!");

		Logger::Log("[Renderer] Create IDXGIFactory1...");

		IDXGIDevice* dxgiDevice = nullptr;
		S2DE_CHECK(m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice), "Render Error: Check failed m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice)");
		IDXGIAdapter* adapter = nullptr;
		S2DE_CHECK(dxgiDevice->GetAdapter(&adapter), "Render Error: Check failed dxgiDevice->GetAdapter(&adapter)");
		S2DE_CHECK(adapter->GetParent(_uuidof(IDXGIFactory1), (void**)&m_factory1), "Render Error: Cannot create DXGI factory");
		Release(adapter);
		Release(dxgiDevice);

#if defined(_DEBUG) && defined(S2DE_DEBUG_RENDER_MODE)
		Logger::Log("[Renderer] Create debug layer...");
		if (SUCCEEDED(m_device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_d3ddebug))))
		{
			S2DE_CHECK_SAFE(m_d3ddebug->SetSwapChain(m_swapChain), "Can't set swap chain");

			typedef HRESULT(__stdcall* fPtr)(const IID&, void**);
			HMODULE hDll = LoadLibraryA("dxgidebug.dll");
			fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");

			IDXGIDebug* debugDev;
			DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&debugDev);
			debugDev->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		
			if (SUCCEEDED(m_device->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&m_d3dinfoqueue))))
			{
					m_d3dinfoqueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
					//m_d3dinfoqueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);

					D3D11_MESSAGE_ID hide[] =
					{
						D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
					};

					D3D11_INFO_QUEUE_FILTER filter = {};
					filter.DenyList.NumIDs = _countof(hide);
					filter.DenyList.pIDList = hide;

					m_d3dinfoqueue->AddStorageFilterEntries(&filter);
			}
			CaptureMessages();
		}

#endif 


		IDXGIFactory2* dxgiFactory2 = nullptr;
		S2DE_CHECK(m_factory1->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2)), "Render Error: Failed to create Factory2");

		if (dxgiFactory2)
		{
			Logger::Log("[Renderer] Create swap chain...");
			S2DE_CHECK(m_device->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_device1)), "Render Error: Failed to create device1");
			m_deviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_deviceContext1));


			//todo 
			//idk how to set vsync for swap chain 

			DXGI_SWAP_CHAIN_DESC1 sd = { };
			//sd.Width = width;
			//sd.Height = height;
			sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.BufferCount = 1;

			S2DE_CHECK(dxgiFactory2->CreateSwapChainForHwnd(m_device, Engine::GetGameWindow()->GetHWND(), &sd, nullptr, nullptr, &m_swapChain1), "Render Error: Failed to create swap chain for hwnd");
			S2DE_CHECK(m_swapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_swapChain)), "Render Error: m_swapChain1 to m_swapChain");
			
			Release(dxgiFactory2);
		}
		else
		{
			Logger::Log("[Renderer] Create swap chain (DirectX 11.0)...");

			DXGI_SWAP_CHAIN_DESC sd = { };
			//sd.BufferDesc.Width  = Engine::GetGameWindow()->GetWidth();
			//sd.BufferDesc.Height = Engine::GetGameWindow()->GetHeight();
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
			sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			sd.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
			sd.BufferCount = 1;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.Windowed = !Engine::GetGameWindow()->isFullscreen();
			sd.OutputWindow = Engine::GetGameWindow()->GetHWND();
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			sd.BufferDesc.RefreshRate.Numerator = m_vsync == true ? m_mode_numerator : 0;
			sd.BufferDesc.RefreshRate.Denominator = m_vsync == true ? m_mode_denominator : 1;

			S2DE_CHECK(m_factory1->CreateSwapChain(m_device, &sd, &m_swapChain), "Render Error: Cannot create swap chain!");
		}

				

		m_factory1->MakeWindowAssociation(Engine::GetGameWindow()->GetHWND(), DXGI_MWA_NO_ALT_ENTER);


		//Create the render target view with the back buffer pointer.
		ID3D11Texture2D* backBufferPtr = nullptr;

		S2DE_CHECK(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferPtr), "Render Error: Get buffer check failed");
		S2DE_CHECK(m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView), "Render Error: Cannot create render target view");

		CreateFramebufferTexture(backBufferPtr);
		Release(backBufferPtr);

		UpdateViewport();

		return true;
	}

	void Renderer::SwitchFillMode(RenderFillMode mode)
	{
		Logger::Log("[Renderer] Switch fill mode %d", (std::int32_t)mode);

		m_render_fill_mode = mode;
		CreateRasterizerState();
	}

	bool Renderer::CreateBlendState()
	{
		D3D11_BLEND_DESC blend_desc = { };
		blend_desc.RenderTarget[0].BlendEnable = true;
		blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		S2DE_CHECK(m_device->CreateBlendState(&blend_desc, &m_blendState), "Can't create blend state");

		m_deviceContext->OMSetBlendState(m_blendState, 0, 0xffffffff);
		return true;
	}

	bool Renderer::CreateRasterizerState()
	{
		if (m_rasterState != nullptr)
		{
			m_deviceContext->RSSetState(nullptr);
			Release(m_rasterState);
		}

		D3D11_RASTERIZER_DESC rasterDesc = { };

		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = static_cast<D3D11_FILL_MODE>(m_render_fill_mode);
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		//Create the rasterizer state from the description we just filled out.
		S2DE_CHECK(m_device->CreateRasterizerState(&rasterDesc, &m_rasterState), "Render Error: Cannot create rasterizer state");


		m_deviceContext->RSSetState(m_rasterState);

		return true;
	}

	bool Renderer::CreateDepthStencil()
	{
		DXGI_SWAP_CHAIN_DESC sd = { };
		m_swapChain->GetDesc(&sd);

		//Set up the description of the depth buffer.
		D3D11_TEXTURE2D_DESC depthBufferDesc = { };
		depthBufferDesc.Width = sd.BufferDesc.Width;
		depthBufferDesc.Height = sd.BufferDesc.Height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		S2DE_CHECK(m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer), "Render Error: Cannot create depth buffer");

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = { };

		//Set up the depth stencil view description.
		depthStencilViewDesc.Format = depthBufferDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		//Create the depth stencil view.
		S2DE_CHECK(m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView), "Render Error: Cannot create depth stencil view");
		m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

		return true;	
	}

	void Renderer::UpdateViewport()
	{
		m_viewport.Width = (float)Engine::GetGameWindow()->GetWidth();
		m_viewport.Height = (float)Engine::GetGameWindow()->GetHeight();
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;

		m_deviceContext->RSSetViewports(1, &m_viewport);
	}

	bool Renderer::Create()
	{
		Logger::Log("[Renderer] [Create Render] Create device and swapchain...");
		if (!CreateDeviceAndSwapChain())
			return false;
		
		Logger::Log("[Renderer] [Create Render] Create depth stencil...");
		if (!CreateDepthStencil())
			return false;

		Logger::Log("[Renderer] [Create Render] Create rasterizer state...");
		if (!CreateRasterizerState())
			return false;
		
		Logger::Log("[Renderer] [Create Render] Create blend state...");
		if (!CreateBlendState())
			return false;

		Logger::Log("[Renderer] [Create Render] Initialize ImGui...");
		if (!InitImGui())
			return false;

		AddImGuiWindow("EngineConsole", new Debug::VisualConsole());

		if(Engine::isEditor())
			GetImGui_Window("EngineConsole")->ToggleDraw();


		AddImGuiWindow("DebugInfoWindow", new Debug_Info());
		AddImGuiWindow("DebugObjectInspectorWindow", new Debug_ObjectInspector());

		if(Core::Engine::CheckAppParam("-DebugInfo"))
			GetImGui_Window("DebugInfoWindow")->ToggleDraw();

		if (Engine::isEditor())
		{
			Logger::Log("[Renderer] Create editor ui...");
			//AddImGuiWindow("EditorRenderWindow", new EditorRenderWindow());
			//GetImGui_Window("EditorRenderWindow")->ToggleDraw();

			AddImGuiWindow("EditorToolStrip", new EditorToolStrip());
			GetImGui_Window("EditorToolStrip")->ToggleDraw();

			AddImGuiWindow("EditorObjectInspector", new EditorObjectInspector());
			GetImGui_Window("EditorObjectInspector")->ToggleDraw(); 

			AddImGuiWindow("EditorBgColorPicker", new EditorColorPicker());

			m_editor_cursor = new Editor::EditorCursor();
			m_editor_center_cursor = new Editor::EditorCenterCursor();
		}

		return true;
	}

	void Renderer::Destroy()
	{
		Logger::Log("[Renderer] Destroy...");

		if (Core::Engine::isEditor())
		{
			Delete(m_editor_cursor);
			Delete(m_editor_center_cursor);
		}

		m_windows_storage.clear();
		m_windows_storage.shrink_to_fit();

		DestroyImGui();

		Release(m_swapChain);
		Release(m_swapChain1);
		Release(m_device);
		Release(m_device1);
		Release(m_deviceContext);
		Release(m_deviceContext1);
		Release(m_renderTargetView);
		Release(m_rasterState);
		Release(m_depthStencilBuffer);
		Release(m_depthStencilState);
		Release(m_depthStencilView);
		Release(m_framebuffer_data);
		Release(m_framebuffer_shaderResourceView);
		Release(m_factory1);
		Release(m_d3dinfoqueue);
		Release(m_d3ddebug);
	}

	void Renderer::Clear()
	{
		float color_array[4] = { m_clearColor.r, m_clearColor.g, m_clearColor.b , m_clearColor.a };

		m_deviceContext->ClearRenderTargetView(m_renderTargetView, color_array);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void Renderer::End()
	{
		m_swapChain->Present((std::uint32_t)m_vsync, 0);
	}

	void Renderer::DestroyImGui()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void Renderer::RenderImGui()
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void Renderer::UpdateImGuiWindows()
	{
		S2DE_IMGUI_NEW_FRAME();

		if (m_show_imgui_windows)
		{
			if (Engine::isEditor())
			{
				ImGui::SetNextWindowPos(ImVec2(0, 0));

				ImGui::SetNextWindowSize(ImVec2(float(Engine::GetGameWindow()->GetWidth()), float(Engine::GetGameWindow()->GetHeight())));

				ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoBringToFrontOnFocus |
					ImGuiWindowFlags_NoNavFocus |
					ImGuiWindowFlags_NoDocking |
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_MenuBar |
					ImGuiWindowFlags_NoBackground |
					ImGuiWindowFlags_NoScrollbar;

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

				bool show = ImGui::Begin("Dockspace", NULL, windowFlags);
				ImGui::PopStyleVar();

				Render::ImGui_Window* toolstrip = GetImGui_Window("EditorToolStrip");

				if (toolstrip != nullptr)
					toolstrip->SetDrawState(show);

				ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

				reinterpret_cast<EditorColorPicker*>(GetImGui_Window("EditorBgColorPicker"))->GetColor(m_clearColor);
				for (std::vector<std::pair<std::string, ImGui_Window*>>::iterator it = m_windows_storage.begin(); it != m_windows_storage.end(); it++)
					it->second->Render();

				ImGui::End();
			}
			else
			{
				for (std::vector<std::pair<std::string, ImGui_Window*>>::iterator it = m_windows_storage.begin(); it != m_windows_storage.end(); it++)
					it->second->Render();
			}



			Engine::GetSceneManager()->RenderImGUI();

			//#if defined(_DEBUG) && defined(S2DE_DEBUG_RENDER_MODE)
			//		if (Engine::isEditor())
			//			ImGui::ShowDemoWindow(&m_show_imgui_demo_wnd);
			//#endif
		}
		ImGui::Render();
	}

	void Renderer::Render()
	{
		UpdateImGuiWindows();

#if defined(_DEBUG) && defined(S2DE_DEBUG_RENDER_MODE)
		CaptureMessages();
#endif

		Clear();

		Engine::GetApplicationHandle()->OnRender();
		Engine::GetSceneManager()->RenderScene();
		
		UpdateFramebufferShaderResource();


		RenderImGui();

		if (Engine::isEditor())
		{
			if (m_framebuffer_shaderResourceView != nullptr && GetImGui_Window("EditorRenderWindow") != nullptr)
				reinterpret_cast<EditorRenderWindow*>(GetImGui_Window("EditorRenderWindow"))->PushRenderTexture((void*)m_framebuffer_shaderResourceView);
			

			m_editor_center_cursor->Render();
			m_editor_cursor->Render();
		}

		End();
	}

	ImGui_Window* Renderer::GetImGui_Window(std::string name) const
	{
		std::vector<std::pair<std::string, ImGui_Window*>>::const_iterator it = std::find_if(m_windows_storage.begin(),
			m_windows_storage.end(), [&name](std::pair<std::string, ImGui_Window*> const& elem) { 
				return elem.first == name;
			});

		if(it != m_windows_storage.end())
			return it->second;

		return nullptr;
	}

	void Renderer::AddImGuiWindow(std::string name, ImGui_Window* wnd)
	{
		if (isStringEmpty(name) || wnd == nullptr)
		{
			Logger::Error("[Renderer] Can't add window to storage!");
			return;
		}

		wnd->SetName(name);

		m_windows_storage.push_back(std::make_pair(name, wnd));
		Logger::Log("[Renderer] Added window %s", name.c_str());
	}

	void Renderer::DeleteImGuiWindow(std::string name)
	{
		std::vector<std::pair<std::string, ImGui_Window*>>::const_iterator it = std::find_if(m_windows_storage.begin(),
			m_windows_storage.end(), [&name](std::pair<std::string, ImGui_Window*> const& elem) {
				return elem.first == name;
			});
		
		m_windows_storage.erase(it, m_windows_storage.end());
		Logger::Log("[Renderer] Removed window %s", name.c_str());
	}

	void Renderer::SetBackColor(Color<float> color)
	{
		m_clearColor = color;
	}

	bool Renderer::CreateFramebufferTexture(ID3D11Texture2D* sw_buff)
	{
		D3D11_TEXTURE2D_DESC td = { };
		sw_buff->GetDesc(&td);
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		S2DE_CHECK(m_device->CreateTexture2D(&td, NULL, &m_framebuffer_data), "Can't create framebuffer texture data");
		return true;
	}

	void Renderer::UpdateFramebufferShaderResource()
	{
		ID3D11Texture2D* pBuffer;
		m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
		m_deviceContext->CopyResource(m_framebuffer_data, pBuffer);
		Release(pBuffer);

		if (m_framebuffer_data == nullptr)
			return;

		D3D11_TEXTURE2D_DESC textureDesc{};
		m_framebuffer_data->GetDesc(&textureDesc);
		
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc {};
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		m_device->CreateShaderResourceView(m_framebuffer_data, &shaderResourceViewDesc, &m_framebuffer_shaderResourceView);
	}

	bool Renderer::CaptureMessages()
	{
		std::int64_t message_count = m_d3dinfoqueue->GetNumStoredMessages();

		for (std::int64_t i = 0; i < message_count; i++)
		{
			SIZE_T message_size = 0;
			m_d3dinfoqueue->GetMessage(i, nullptr, &message_size); 
			//D3D11_MESSAGE_SEVERITY
			D3D11_MESSAGE* message = (D3D11_MESSAGE*)malloc(message_size); 
			S2DE_CHECK_SAFE(m_d3dinfoqueue->GetMessageA(i, message, &message_size), "Debug layer message get error!");

			switch (message->Severity)
			{
			case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_ERROR:
				Logger::Error("[Renderer] [D3D11] ERROR %.*s", message->DescriptionByteLength, message->pDescription);
				break;
			case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_CORRUPTION:
				Logger::Error("[Renderer] [D3D11] ! CORRUPTION ! %.*s", message->DescriptionByteLength, message->pDescription);
				break;
			case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_WARNING:
				Logger::Warning("[Renderer] [D3D11] WARNING %.*s", message->DescriptionByteLength, message->pDescription);
				break;
			default:
				Logger::Log("[Renderer] [D3D11] %.*s", message->DescriptionByteLength, message->pDescription);
				break;
			}

			free(message);
		}

		m_d3dinfoqueue->ClearStoredMessages();

		return true;
	}

	void Renderer::ToggleImGuiDemoWindowVisible()
	{
		m_show_imgui_demo_wnd =! m_show_imgui_demo_wnd;
	}

	void Renderer::ToggleImGuiWindowsVisible()
	{
		m_show_imgui_windows =! m_show_imgui_windows;
	}

	void Renderer::TurnZBufferOn()
	{
		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
		return;
	}

	void Renderer::TurnZBufferOff()
	{
		m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
		return;
	}
}