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
							m_rasterStateCW(nullptr),
							m_rasterStateCCW(nullptr),
							m_depthStencilBuffer(nullptr),
							m_depthStencilState(nullptr),
							m_depthStencilView(nullptr),
							m_blendState(nullptr),
							m_vsync(true),
							m_show_imgui_windows(true),
							m_show_imgui_demo_wnd(false),
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
		Release(m_renderTargetView);
		Release(m_backBuffer);
		Release(m_depthStencilBuffer);
		Release(m_depthStencilView);
		m_deviceContext->Flush();
		S2DE_CHECK(m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0), "Can't resize buffers");

		ConfigureBackBuffer();
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
		ImGui_ImplDX11_Init(m_device, m_deviceContext);

		return true;
	}

	bool Renderer::ConfigureBackBuffer()
	{
		m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_backBuffer);
		S2DE_CHECK(m_device->CreateRenderTargetView(m_backBuffer, NULL, &m_renderTargetView), "Render Error: Cannot create render target view");
		CreateFramebufferTexture(m_backBuffer);
		return true;
	}

	bool Renderer::CreateDeviceAndSwapChain()
	{
		Logger::Log("[Renderer] Create device and swap chain...");

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

		//If defined S2DE_DEBUG_RENDER_MODE macro and if it's debug build 
		//We are add D3D11_CREATE_DEVICE_DEBUG to device
#if defined(_DEBUG) && defined(S2DE_DEBUG_RENDER_MODE)
		Logger::Warning("[Renderer] Render in debug mode!");
		m_device_flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		std::int32_t driver_types_size = ARRAYSIZE(driverTypes);
		HRESULT device_hr = S_OK;

		DXGI_SWAP_CHAIN_DESC sd = { };
		sd.BufferDesc.Width = Core::Engine::GetGameWindow()->GetWidthFixed();
		sd.BufferDesc.Height = Core::Engine::GetGameWindow()->GetHeightFixed();
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferCount = 1;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = !Engine::GetGameWindow()->isFullscreen();
		sd.OutputWindow = Engine::GetGameWindow()->GetHWND();
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 1;


		//Try to create device with correct driver type 
		for (std::uint32_t driverTypeIndex = 0; driverTypeIndex < driver_types_size; driverTypeIndex++)
		{
			D3D_DRIVER_TYPE driverType = driverTypes[driverTypeIndex];
			device_hr = D3D11CreateDeviceAndSwapChain(nullptr, driverType, nullptr, m_device_flag, FeatureLevels, ARRAYSIZE(FeatureLevels), D3D11_SDK_VERSION, &sd, &m_swapChain, &m_device, nullptr, &m_deviceContext);

			//If we get E_INVALIDARG probably we are need to use D3D_FEATURE_LEVEL_11_0 
			if (device_hr == E_INVALIDARG)
				device_hr = D3D11CreateDeviceAndSwapChain(nullptr, driverType, nullptr, m_device_flag, FeatureLevels, ARRAYSIZE(FeatureLevels), D3D11_SDK_VERSION, &sd, &m_swapChain, &m_device, nullptr, &m_deviceContext);

			if (SUCCEEDED(device_hr)) //If creation device succeeded we are break that loop
				break;
		}

		//If device not created we are get fatal error 
		if (m_device == nullptr)
			S2DE_FATAL_ERROR("Render Error: Can't create device and swap chain!");


		//Debug layer creation 
#if defined(_DEBUG) && defined(S2DE_DEBUG_RENDER_MODE)
		Logger::Log("[Renderer] Create debug layer...");
		if (SUCCEEDED(m_device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_d3ddebug))))
		{			
			typedef HRESULT(__stdcall* fPtr)(const IID&, void**);
			HMODULE hDll = LoadLibraryA("dxgidebug.dll");
			fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");

			IDXGIDebug* debugDev;
			DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&debugDev);
			debugDev->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		
			if (SUCCEEDED(m_device->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&m_d3dinfoqueue))))
			{
				m_d3dinfoqueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
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


		return true;
	}

	void Renderer::SwitchFillMode(RenderFillMode mode)
	{
		Logger::Log("[Renderer] Switch fill mode to %s", mode == RenderFillMode::Solid ? "Solid" : "Wireframe");

		m_render_fill_mode = mode;
		CreateRasterizerState();
	}

	bool Renderer::CreateBlendState()
	{
		S2DE_NO_IMPL();
		return true;
	}

	bool Renderer::CreateRasterizerState()
	{
		//If we need to update rasterizer
		if (m_rasterStateCW != nullptr || m_rasterStateCCW != nullptr)
		{
			m_deviceContext->RSSetState(nullptr);
			Release(m_rasterStateCW);
			Release(m_rasterStateCCW);
		}

		D3D11_RASTERIZER_DESC rasterDesc = { };

		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = static_cast<D3D11_FILL_MODE>(m_render_fill_mode);
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		//Create the rasterizer state from the description we just filled out.
		rasterDesc.FrontCounterClockwise = true;
		S2DE_CHECK(m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateCCW), "Render Error: Cannot create rasterizer state");

		rasterDesc.FrontCounterClockwise = false;
		S2DE_CHECK(m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateCW), "Render Error: Cannot create rasterizer state");
		return true;
	}

	bool Renderer::CreateDepthStencil()
	{
		std::uint32_t w = Core::Engine::GetGameWindow()->GetWidthFixed();
		std::uint32_t h = Core::Engine::GetGameWindow()->GetHeightFixed();

		//Set up the description of the depth buffer.
		D3D11_TEXTURE2D_DESC depthBufferDesc = { };
		depthBufferDesc.Width = w;
		depthBufferDesc.Height = h;
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

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = { };
		//Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;
		
		//Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		
		//Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		
		S2DE_CHECK(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState), "Render Error: Cannot create depth stencil state");
		
		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 0);

		//Set up the depth stencil view description.
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = { };
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
		//TODO: I need get it from swap chain or from window ?
		m_viewport.Width	= (float)Core::Engine::GetGameWindow()->GetWidthFixed();
		m_viewport.Height	= (float)Core::Engine::GetGameWindow()->GetHeightFixed();
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;

		m_deviceContext->RSSetViewports(1, &m_viewport);
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

		if (!ConfigureBackBuffer())
			return false;

		Logger::Log("[Renderer] [Create Render] Create depth stencil...");
		if (!CreateDepthStencil())
			return false;

		UpdateViewport();

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
		AddImGuiWindow("DebugInfoWindow", new Debug_Info());
		AddImGuiWindow("DebugObjectInspectorWindow", new Debug_ObjectInspector());

		GetImGui_Window("DebugInfoWindow")->SetDrawState(Core::Engine::CheckAppParam("-DebugInfo"));

		if (Engine::isEditor())
		{
			Logger::Log("[Renderer] Create editor ui...");

			//AddImGuiWindow("EditorRenderWindow", new EditorRenderWindow(), true);
			AddImGuiWindow("EditorObjectInspector", new EditorObjectInspector(), false);
			AddImGuiWindow("EditorBgColorPicker", new EditorColorPicker(), true);
			
			m_editor_cursor = new Editor::EditorCursor();
			m_editor_center_cursor = new Editor::EditorCenterCursor();

			m_editor_toolstrip = new EditorToolStrip();
			m_editor_toolstrip->SetDrawState(true);
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
		Release(m_device);
		Release(m_deviceContext);
		Release(m_renderTargetView);

		Release(m_rasterStateCW);
		Release(m_rasterStateCCW);

		Release(m_depthStencilBuffer);
		Release(m_depthStencilState);
		Release(m_depthStencilView);
		Release(m_framebuffer_data);
		Release(m_framebuffer_shaderResourceView);
		Release(m_d3dinfoqueue);
		Release(m_d3ddebug);
	}

	void Renderer::Clear()
	{
		float color_array[4] = { m_clearColor.r, m_clearColor.g, m_clearColor.b , 1 };

		m_deviceContext->ClearRenderTargetView(m_renderTargetView, color_array);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
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

			if (m_show_imgui_windows)
				for (const auto& it : m_windows_storage)
					it.second->Render();

			m_editor_toolstrip->SetDrawState(show);
			m_editor_toolstrip->Render();

			ImGui::End();
			
		}
		else
		{
			for (const auto& it : m_windows_storage)
				it.second->Render();
		}



		Engine::GetSceneManager()->RenderImGUI();

		#if defined(_DEBUG) && defined(S2DE_DEBUG_RENDER_MODE)
				if (Engine::isEditor() && m_show_imgui_demo_wnd)
					ImGui::ShowDemoWindow(&m_show_imgui_demo_wnd);
		#endif
		
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


		if (Engine::isEditor())
			m_editor_center_cursor->Render();

		RenderImGui();

		if (Engine::isEditor())
		{
			EditorColorPicker* colorPicker = reinterpret_cast<EditorColorPicker*>(GetImGui_Window("EditorBgColorPicker"));

			if(colorPicker != nullptr)
				colorPicker->SetColor(m_clearColor);

			if (m_framebuffer_shaderResourceView != nullptr && GetImGui_Window("EditorRenderWindow") != nullptr)
				reinterpret_cast<EditorRenderWindow*>(GetImGui_Window("EditorRenderWindow"))->PushRenderTexture((void*)m_framebuffer_shaderResourceView);			

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

	void Renderer::AddImGuiWindow(std::string name, ImGui_Window* wnd, bool visible)
	{
		if (isStringEmpty(name) || wnd == nullptr)
		{
			Logger::Error("[Renderer] Can't add window to storage!");
			return;
		}

		wnd->SetName(name);
		wnd->SetDrawState(visible);

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
			D3D11_MESSAGE* message = (D3D11_MESSAGE*)malloc(message_size); 
			S2DE_CHECK_SAFE(m_d3dinfoqueue->GetMessageA(i, message, &message_size), "Debug layer message get error!");

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
	}

	void Renderer::TurnZBufferOff()
	{
		m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
	}
}