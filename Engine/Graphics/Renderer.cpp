#include "Renderer.h"

#include "Base/Engine.h"
#include "Base/DebugTools/VisualConsole.h"
#include "Base/ApplicationHandle.h"
#include "Base/GameWindow.h"

#include "Scene/SceneManager.h"

#include "Graphics/ImGui_Window.h"
#include "Graphics/ImGuiS2DETheme.h"

#include "Base/DebugTools/Debug_Info.h"
#include "Base/DebugTools/Debug_ObjectInspector.h"

#include "Editor/EditorToolstrip.h"
#include "Editor/EditorObjectInspector.h"
#include "Editor/EditorRenderWindow.h"
#include "Editor/EditorColorPicker.h"

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
							m_context(nullptr),
							m_targetView(nullptr),
							m_depthStencilBuffer(nullptr),
							m_depthStateEnabled(nullptr),
							m_depthStateDisabled(nullptr),
							m_depthStencilView(nullptr),
							m_blendStateOn(nullptr),
							m_blendStateOff(nullptr),	
							m_vsync(true),
							m_showImguiWindows(true),
							m_showImguiDemoWindow(false),
							m_deviceFlags(0),
							m_fillMode(RenderFillMode::Solid),
							m_clearColor(Color<float>(0.0f, 0.0f, 0.0f, 1.0f))

	{

	}


	Renderer::~Renderer()
	{

	}

	bool Renderer::Create()
	{
		if (!DirectX::XMVerifyCPUSupport())
		{
			S2DE_FATAL_ERROR("Render Error: XNA Math is not supported on this CPU");
			return false;
		}

		Logger::Log("[Renderer] [Create Render] Create device and swapchain...");
		if (!CreateDeviceAndSwapChain())
			return false;

#if defined(_DEBUG) && defined(S2DE_DEBUG_RENDER_MODE)
		CreateDebugLayer();
#endif

		UpdateViewport();

		Logger::Log("[Renderer] [Create Render] Create default rasterizer state...");
		if (!CreateRasterizerState())
			return false;

		//FIX ME: Fillmode is must be independent
		D3D11_RASTERIZER_DESC wireframeDesc = defaultRasterDesc;
		wireframeDesc.FillMode = static_cast<D3D11_FILL_MODE>(RenderFillMode::Wireframe);
		if (!CreateRasterizerState(wireframeDesc, "wireframe"))
			return false;

		D3D11_RASTERIZER_DESC fccDesc = defaultRasterDesc;
		fccDesc.FrontCounterClockwise = true;
		if (!CreateRasterizerState(fccDesc, "fcc"))
			return false;

		D3D11_RASTERIZER_DESC nocullDesc = defaultRasterDesc;
		nocullDesc.CullMode = D3D11_CULL_NONE;
		if (!CreateRasterizerState(nocullDesc, "nocull"))
			return false;

		SetRasterizerState();

		Logger::Log("[Renderer] [Create Render] Create depth stencil and render target...");
		if (!CreateDepthStencil())
			return false;

		Logger::Log("[Renderer] [Create Render] Create blend state...");
		if (!CreateBlendState())
			return false;

		Logger::Log("[Renderer] [Create Render] Initialize ImGui...");
		if (!InitImGui())
			return false;

		CreateEngineWindowsAndEditorUI();

		return true;
	}

	void Renderer::CreateEngineWindowsAndEditorUI()
	{
		AddImGuiWindow("EngineConsole", new Debug::VisualConsole());
		AddImGuiWindow("DebugInfoWindow", new Debug_Info());

		if (Engine::isEditor())
		{
			//AddImGuiWindow("EditorRenderWindow", new EditorRenderWindow(), true);
			AddImGuiWindow("EditorObjectInspector", new EditorObjectInspector(), true);
			AddImGuiWindow("EditorBgColorPicker", new EditorColorPicker(), true);


			m_editorToolStrip = new EditorToolStrip();
			m_editorToolStrip->SetDrawState(true);
		}
	}

	bool Renderer::Reset()
	{
		Release(m_targetView);
		Release(m_backBuffer);
		Release(m_depthStencilBuffer);
		Release(m_depthStencilView);
		m_context->Flush();
		S2DE_CHECK(m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0), "Can't resize buffers");

		CreateRenderTarget();
		CreateDepthStencil();
		UpdateViewport();

		return true;
	}

	void Renderer::LoadCustomImguiTheme()
	{
		SetS2DETheme();
	}

	bool Renderer::InitImGui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); 
		io.ConfigFlags = Engine::isEditor() ? ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NoMouseCursorChange 
			: ImGuiConfigFlags_NoMouseCursorChange;

		//Search custom font
		std::string path = std::string();
		if (Engine::GetResourceManager().GetFilePath(S2DE_DEFAULT_FONT_NAME, "Font", ".ttf", path))
			io.Fonts->AddFontFromFileTTF(path.c_str(), 16);
		
		LoadCustomImguiTheme();

		//Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(Engine::GetGameWindow()->GetHWND());
		ImGui_ImplDX11_Init(m_device, m_context);

		return true;
	}

	void Renderer::CreateDebugLayer()
	{
		if (SUCCEEDED(m_device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_d3dDebug))))
		{
			HMODULE hDll = LoadLibraryA("dxgidebug.dll");

			if (hDll == 0)
			{
				Logger::Warning("Can't create debug layer because could not load library dxgidebug.dll... ");
				return;
			}

			typedef HRESULT(__stdcall* fPtr)(const IID&, void**);
			fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");

			IDXGIDebug* debugDev;
			DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&debugDev);
			debugDev->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);

			if (SUCCEEDED(m_device->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&m_d3dInfoQueue))))
			{
				m_d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
				D3D11_MESSAGE_ID hide[] =
				{
					D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				};

				D3D11_INFO_QUEUE_FILTER filter = {};
				filter.DenyList.NumIDs = _countof(hide);
				filter.DenyList.pIDList = hide;

				m_d3dInfoQueue->AddStorageFilterEntries(&filter);
			}

			CaptureMessages();
		}
	}

	bool Renderer::CreateRenderTarget()
	{
		m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_backBuffer);
		S2DE_CHECK(m_device->CreateRenderTargetView(m_backBuffer, NULL, &m_targetView), "Render Error: Cannot create render target view");
		CreateFramebufferTexture(m_backBuffer);
		return true;
	}

	bool Renderer::CreateDeviceAndSwapChain()
	{
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

		//If defined S2DE_DEBUG_RENDER_MODE macro and if it's debug build, we are add D3D11_CREATE_DEVICE_DEBUG flag to device
#if defined(_DEBUG) && defined(S2DE_DEBUG_RENDER_MODE)
		m_deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		std::uint32_t driver_types_size = ARRAYSIZE(driverTypes);
		HRESULT device_hr = S_OK;

		DXGI_SWAP_CHAIN_DESC sd = { };
		sd.BufferDesc.Width = Core::Engine::GetGameWindow()->GetWidthFixed();
		sd.BufferDesc.Height = Core::Engine::GetGameWindow()->GetHeightFixed();
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.BufferCount = 1;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = !Engine::GetGameWindow()->isFullscreen();
		sd.OutputWindow = Engine::GetGameWindow()->GetHWND();
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		//Try to create device with correct driver type 
		for (std::uint32_t driverTypeIndex = 0; driverTypeIndex < driver_types_size; driverTypeIndex++)
		{
			D3D_DRIVER_TYPE driverType = driverTypes[driverTypeIndex];
			device_hr = D3D11CreateDeviceAndSwapChain(nullptr, driverType, nullptr, m_deviceFlags, FeatureLevels, ARRAYSIZE(FeatureLevels), D3D11_SDK_VERSION, &sd, &m_swapChain, &m_device, nullptr, &m_context);

			//If we get E_INVALIDARG probably we are need to use D3D_FEATURE_LEVEL_11_0 
			if (device_hr == E_INVALIDARG)
				device_hr = D3D11CreateDeviceAndSwapChain(nullptr, driverType, nullptr, m_deviceFlags, FeatureLevels, ARRAYSIZE(FeatureLevels), D3D11_SDK_VERSION, &sd, &m_swapChain, &m_device, nullptr, &m_context);

			if (SUCCEEDED(device_hr)) //If creation device succeeded we are break that loop
				break;
		}

		//If device not created we are get fatal error 
		if (m_device == nullptr)
			S2DE_FATAL_ERROR("Render Error: Can't create device and swap chain!");

		return true;
	}

	void Renderer::SwitchFillMode(RenderFillMode mode)
	{
		Logger::Log("[Renderer] Switch fill mode to %s", mode == RenderFillMode::Solid ? "Solid" : "Wireframe");

		m_fillMode = mode;
		switch (m_fillMode)
		{
		case S2DE::Render::RenderFillMode::Solid:
			SetRasterizerState("default");
			break;
		case S2DE::Render::RenderFillMode::Wireframe:
			SetRasterizerState("wireframe");
			break;
		}
	}

	bool Renderer::CreateBlendState()
	{
		D3D11_BLEND_DESC bd = { };
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX; // D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA; //D3D11_BLEND_ONE;

		S2DE_CHECK(m_device->CreateBlendState(&bd, &m_blendStateOn), "Render Error: Can't create blend state on");

		bd.RenderTarget[0].BlendEnable = false;
		S2DE_CHECK(m_device->CreateBlendState(&bd, &m_blendStateOff), "Render Error: Can't create blend state off");

		return true;
	}

	void Renderer::TurnOffAlphaBlending()
	{
		float factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_context->OMSetBlendState(m_blendStateOff, factor, 0xffffffff);
	}

	void Renderer::TurnOnAlphaBlending()
	{
		float factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_context->OMSetBlendState(m_blendStateOn, factor, 0xffffffff);
	}

	bool Renderer::CreateRasterizerState(D3D11_RASTERIZER_DESC desc, std::string name)
	{
		//We cannot rewrite default rasterizer variant
		if (GetRasterizerState("default") != nullptr && name == "default")
			return false;

		ID3D11RasterizerState* newRasterizer = nullptr;

		// Create the rasterizer state from the description we just filled out.		
		S2DE_CHECK_SAFE(m_device->CreateRasterizerState(&desc, &newRasterizer), "Render Error: Cannot create rasterizer state");

		// Push new rasterizer variant to storage
		m_rasterizerVariants.push_back(std::make_pair(name, newRasterizer));

		return true;
	}

	bool Renderer::CreateDepthStencil()
	{
		if (!CreateRenderTarget())
			return false;

		std::uint32_t w = Core::Engine::GetGameWindow()->GetWidthFixed();
		std::uint32_t h = Core::Engine::GetGameWindow()->GetHeightFixed();

		//Set up the description of the depth buffer.
		D3D11_TEXTURE2D_DESC depthBufferDesc = { };
		depthBufferDesc.Width = w;
		depthBufferDesc.Height = h;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;//DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		S2DE_CHECK(m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer), "Render Error: Cannot create depth buffer");

		//Set up the description of the stencil state.
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = { };
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		//FIX ME: Stencil temporarily is disabled
		//depthStencilDesc.StencilEnable = false;
		//depthStencilDesc.StencilReadMask = 0xFF;
		//depthStencilDesc.StencilWriteMask = 0xFF;

		//Stencil operations if pixel is front-facing.
		//depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		//depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		//depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		//depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		//Stencil operations if pixel is back-facing.
		//depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		//depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		//depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		//depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		
		S2DE_CHECK(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStateEnabled), "Render Error: Cannot create enabled depth stencil state");

		depthStencilDesc.DepthEnable = false;
		S2DE_CHECK(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStateDisabled), "Render Error: Cannot create disabled depth stencil state");

		//Set up the depth stencil view description.
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = { };
		depthStencilViewDesc.Format = depthBufferDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		//Create the depth stencil view.
		S2DE_CHECK(m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView), "Render Error: Cannot create depth stencil view");

		m_context->OMSetRenderTargets(1, &m_targetView, m_depthStencilView);
		return true;	
	}

	void Renderer::UpdateViewport()
	{
		//TODO: I need get it from swap chain or from window ?
		m_viewport.Width	= (float)Core::Engine::GetGameWindow()->GetWidthFixed();
		m_viewport.Height	= (float)Core::Engine::GetGameWindow()->GetHeightFixed();
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;

		m_context->RSSetViewports(1, &m_viewport);
	}

	void Renderer::Destroy()
	{
		Logger::Log("[Renderer] Destroy...");

		if (Core::Engine::isEditor())
			Delete(m_editorToolStrip);
		

		m_windowsStorage.clear();
		m_windowsStorage.shrink_to_fit();

		DestroyImGui();

		m_rasterizerVariants.clear();
		m_rasterizerVariants.shrink_to_fit();

		Release(m_swapChain);
		Release(m_device);
		Release(m_context);
		Release(m_targetView);
		Release(m_depthStencilBuffer);
		Release(m_depthStateEnabled);
		Release(m_depthStateDisabled);
		Release(m_depthStencilView);
		Release(m_frameBufferData);
		Release(m_frameBufferShaderResourceView);

#if defined(_DEBUG) && defined(S2DE_DEBUG_RENDER_MODE)
		m_d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
		Release(m_d3dInfoQueue);
		Release(m_d3dDebug);
#endif

	}

	void Renderer::Clear()
	{
		float color_array[4] = { m_clearColor.r, m_clearColor.g, m_clearColor.b , 1 };
		m_context->OMSetDepthStencilState(m_depthStateEnabled, 0);
		m_context->ClearRenderTargetView(m_targetView, color_array);
		m_context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
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
			ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);	

			if (m_showImguiWindows)
				for (const auto& it : m_windowsStorage)
					it.second->Render();

			m_editorToolStrip->SetDrawState(show);
			m_editorToolStrip->Render();

			ImGui::End();
			
		}
		else
		{
			for (const auto& it : m_windowsStorage)
				it.second->Render();
		}



		Engine::GetSceneManager()->RenderImGUI();

		#if defined(_DEBUG) && defined(S2DE_DEBUG_RENDER_MODE)
				if (Engine::isEditor() && m_showImguiDemoWindow)
					ImGui::ShowDemoWindow(&m_showImguiDemoWindow);
		#endif
		
		ImGui::Render();
	}

	inline ID3D11RasterizerState* Renderer::GetRasterizerState(std::string name)
	{
		std::vector<std::pair<std::string, CComPtr<ID3D11RasterizerState>>>::const_iterator it = std::find_if(m_rasterizerVariants.begin(),
			m_rasterizerVariants.end(), [&name](std::pair<std::string, CComPtr<ID3D11RasterizerState>> const& elem) { return elem.first == name; });

		if (it != m_rasterizerVariants.end())
			return it->second;

		return nullptr;
	}

	void Renderer::SetRasterizerState(ID3D11RasterizerState* raster)
	{
		if(raster != nullptr)
			m_context->RSSetState(raster);
	}

	void Renderer::SetRasterizerState(std::string name)
	{
		std::vector<std::pair<std::string, CComPtr<ID3D11RasterizerState>>>::const_iterator it = std::find_if(m_rasterizerVariants.begin(),
			m_rasterizerVariants.end(), [&name](std::pair<std::string, CComPtr<ID3D11RasterizerState>> const& elem) { return elem.first == name; });

		if (it != m_rasterizerVariants.end())
			m_context->RSSetState(it->second);
	}

	void Renderer::Render()
	{
		UpdateImGuiWindows();

#if defined(_DEBUG) && defined(S2DE_DEBUG_RENDER_MODE)
		CaptureMessages();
#endif

		Clear();
		{
			Engine::GetApplicationHandle()->OnRender();
			Engine::GetSceneManager()->RenderScene();

			UpdateFramebufferShaderResource();



			RenderImGui();

			if (Engine::isEditor())
			{
				EditorColorPicker* colorPicker = reinterpret_cast<EditorColorPicker*>(GetImGui_Window("EditorBgColorPicker"));

				if (colorPicker != nullptr)
					colorPicker->SetColor(m_clearColor);

				//if (m_FramebufferShaderResourceView != nullptr && GetImGui_Window("EditorRenderWindow") != nullptr)
				//	reinterpret_cast<EditorRenderWindow*>(GetImGui_Window("EditorRenderWindow"))->PushRenderTexture((void*)m_framebuffer_shaderResourceView);
			}
		}

		End();
	}

	ImGui_Window* Renderer::GetImGui_Window(std::string name) const
	{
		std::vector<std::pair<std::string, ImGui_Window*>>::const_iterator it = std::find_if(m_windowsStorage.begin(),
			m_windowsStorage.end(), [&name](std::pair<std::string, ImGui_Window*> const& elem) { 
				return elem.first == name;
			});

		if(it != m_windowsStorage.end())
			return it->second;

		return nullptr;
	}

	ImGui_Window* Renderer::AddImGuiWindow(std::string name, ImGui_Window* wnd, bool visible)
	{
		if (isStringEmpty(name) || wnd == nullptr)
		{
			Logger::Error("[Renderer] Can't add window to storage!");
			return nullptr;
		}

		wnd->SetName(name);
		wnd->SetDrawState(visible);

		m_windowsStorage.push_back(std::make_pair(name, wnd));
		Logger::Log("[Renderer] Added window %s", name.c_str());
		return wnd;
	}

	void Renderer::DeleteImGuiWindow(std::string name)
	{
		std::vector<std::pair<std::string, ImGui_Window*>>::const_iterator it = std::find_if(m_windowsStorage.begin(),
			m_windowsStorage.end(), [&name](std::pair<std::string, ImGui_Window*> const& elem) {
				return elem.first == name;
			});
		
		m_windowsStorage.erase(it, m_windowsStorage.end());
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
		S2DE_CHECK(m_device->CreateTexture2D(&td, NULL, &m_frameBufferData), "Can't create framebuffer texture data");
		return true;
	}

	void Renderer::UpdateFramebufferShaderResource()
	{
		ID3D11Texture2D* pBuffer = nullptr;
		m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
		if (pBuffer == nullptr)
			return;
		
		m_context->CopyResource(m_frameBufferData, pBuffer);
		Release(pBuffer);

		if (m_frameBufferData == nullptr)
			return;

		D3D11_TEXTURE2D_DESC textureDesc{};
		m_frameBufferData->GetDesc(&textureDesc);
		
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc {};
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		m_device->CreateShaderResourceView(m_frameBufferData, &shaderResourceViewDesc, &m_frameBufferShaderResourceView);
	}

	bool Renderer::CaptureMessages()
	{
		std::int64_t message_count = m_d3dInfoQueue->GetNumStoredMessages();

		for (std::int64_t i = 0; i < message_count; i++)
		{
			SIZE_T message_size = 0;
			m_d3dInfoQueue->GetMessage(i, nullptr, &message_size); 
			D3D11_MESSAGE* message = (D3D11_MESSAGE*)malloc(message_size);
			if (message == nullptr) //If malloc failed
				return false;
			S2DE_CHECK_SAFE(m_d3dInfoQueue->GetMessageA(i, message, &message_size), "Can't get message");

			switch (message->Severity)
			{
				case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_ERROR:
					Logger::Error("[D3D11] ERROR %.*s", message->DescriptionByteLength, message->pDescription);
					break;
				case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_CORRUPTION:
					Logger::Error("[D3D11] CORRUPTION %.*s", message->DescriptionByteLength, message->pDescription);
					break;
				case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_WARNING:
					Logger::Warning("[D3D11] WARNING %.*s", message->DescriptionByteLength, message->pDescription);
					break;
				default:
					Logger::Log("[D3D11] %.*s", message->DescriptionByteLength, message->pDescription);
					break;
			}

			free(message);
		}

		m_d3dInfoQueue->ClearStoredMessages();

		return true;
	}

	void Renderer::ToggleImGuiDemoWindowVisible()
	{
		m_showImguiDemoWindow =!m_showImguiDemoWindow;
	}

	void Renderer::ToggleImGuiWindowsVisible()
	{
		m_showImguiWindows =! m_showImguiWindows;
	}

	void Renderer::TurnZBufferOn()
	{
		m_context->OMSetDepthStencilState(m_depthStateEnabled, 1);
	}

	void Renderer::TurnZBufferOff()
	{
		m_context->OMSetDepthStencilState(m_depthStateDisabled, 1);
	}

	void Renderer::DrawIndexed(std::uint64_t indexCount, std::uint32_t startIndexLocation, std::uint32_t baseVertexLocation, D3D11_PRIMITIVE_TOPOLOGY topology)
	{
		m_context->IASetPrimitiveTopology(topology);
		m_context->DrawIndexed((std::uint32_t)indexCount, startIndexLocation, baseVertexLocation);
	}

	void Renderer::Draw(std::uint64_t vertexCount, std::uint32_t startVertexLocation, D3D11_PRIMITIVE_TOPOLOGY topology)
	{
		m_context->IASetPrimitiveTopology(topology);
		m_context->Draw((std::uint32_t)vertexCount, startVertexLocation);
	}
}