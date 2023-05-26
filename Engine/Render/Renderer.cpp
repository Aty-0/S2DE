#include "Renderer.h"

#include "Base/Engine.h"
#include "Base/DebugTools/Console.h"
#include "Base/ApplicationHandle.h"
#include "Base/GameWindow.h"

#include "Scene/SceneManager.h"

#include "Render/ImGui_Window.h"
#include "Render/ImGuiS2DETheme.h"
#include "Render/FBX_Importer.h"
#include "Render/LightGlobals.h"
#include "Render/Buffers.h"
#include "Render/Shader.h"
#include "Render/Font.h"

#include "GameObjects/Components/Transform.h"

#include "Editor/EditorToolstrip.h"
#include "Editor/EditorObjectInspector.h"
#include "Editor/EditorRenderWindow.h"
#include "Editor/EditorColorPicker.h"
#include "Editor/EditorModelExporterWindow.h"

#include <dxgidebug.h>

#define S2DE_IMGUI_NEW_FRAME()  ImGui_ImplDX11_NewFrame(); \
								ImGui_ImplSDL2_NewFrame(); \
								ImGui::NewFrame(); \

namespace S2DE::Render
{
	Renderer::Renderer() : 
							m_editorToolStrip(nullptr),
							m_editorCenterCursor(nullptr)

	{
		// Initialize structures
		m_base = Renderer::Base
		{
			nullptr, // SwapChain
			nullptr, // Device
			nullptr, // Context
			nullptr, // TargetView
			{0,0,0,0},	// Viewport
			0			// DeviceFlags
		};

		m_zBuffer = Renderer::ZBuffer
		{
			nullptr, // BackBuffer
			nullptr, // DepthStencilBuffer
			nullptr, // DepthStencilView
			{ nullptr, nullptr }, // DepthState[2];
		};

		m_blend = Renderer::Blend
		{
			{ nullptr, nullptr }, // BlendState[2]
		};


		m_framebuffer = Renderer::Framebuffer
		{
			nullptr, // BufferData
			nullptr, // BufferSRV
			nullptr, // RenderTarget
		},

		m_debug = Renderer::Debug
		{
			nullptr, // DebugCore(nullptr)
			nullptr // InfoQueue(nullptr)
		};


		m_params = Renderer::Params
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f }, // ClearColor
			Api::FillMode(Api::FillMode::Type::Solid), // FillMode
			false, // Vsync
			true, // ShowImguiWindows
			true, // ShowImguiDemoWindow
		};
	}


	Renderer::~Renderer()
	{

	}

	bool Renderer::Create()
	{
		if (!DirectX::XMVerifyCPUSupport())
		{
			S2DE_FATAL_ERROR("XNA Math is not supported on this CPU!");
			return false;
		}

		Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0, 0, 1, 1), 
			"Create device and swapchain...");

		if (!CreateDeviceAndSwapChain())
			return false;

#if defined(S2DE_DEBUG_RENDER_MODE)
		Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0, 0, 1, 1),
			"Create debug layer...");

		CreateDebugLayer();
#endif

		UpdateViewport();

		Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0, 0, 1, 1),
			"Create rasterizer states...");

		if (!CreateRasterizerStates())
			return false;

		Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0, 0, 1, 1), 
			"Create depth stencil and render target...");

		if (!CreateDepthStencil())
			return false;

		Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0, 0, 1, 1), 
			"Create blend state...");

		if (!CreateBlendState())
			return false;

		Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0, 0, 1, 1), 
			"Initialize ImGui...");

		if (!InitImGui())
			return false;

		Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0, 0, 1, 1), 
			"Initialize fbx sdk...");

		FBX_Importer::GetInstance()->Init();

		Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0, 0, 1, 1),
			"Initialize lights globals...");

		CreateEngineWindowsAndEditorUI();
		LightGlobals::GetInstance()->Initialize();

		return true;
	}

	void Renderer::CreateEngineWindowsAndEditorUI()
	{
		AddImGuiWindow("Console", new Core::Debug::Console());

		if (Core::Engine::isEditor())
		{
			AddImGuiWindow("EditorRenderWindow", new Editor::EditorRenderWindow(), true);
			AddImGuiWindow("EditorObjectInspector", new Editor::EditorObjectInspector(), true);
			AddImGuiWindow("EditorBgColorPicker", new Editor::EditorColorPicker(), false);
			AddImGuiWindow("EditorModelExporterWindow", new Editor::EditorModelExporterWindow(), true);


			m_editorToolStrip = new Editor::EditorToolStrip();
			m_editorToolStrip->SetDrawState(true);
		}
	}

	bool Renderer::Reset()
	{
		m_base.Context->OMSetRenderTargets(0, nullptr, nullptr);

		Core::Release(m_base.RenderTarget);
		Core::Release(m_framebuffer.RenderTarget);

		Core::Release(m_framebuffer.BufferSRV);
		
		Core::Release(m_zBuffer.BackBuffer);			
		Core::Release(m_zBuffer.DepthStencilView);			
		Core::Release(m_zBuffer.DepthState[1]);
		Core::Release(m_zBuffer.DepthStencilBuffer);
		Core::Release(m_zBuffer.DepthState[0]);

		Core::Release(m_framebuffer.BufferData);

		m_base.Context->Flush();
		
		auto hr = m_base.SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// TODO: What's we need to do with device in this case ?
			S2DE_FATAL_ERROR("Device was removed or was reseted");
			return false;
		}	
		else if(!Core::Utils::Logger::CheckHR(hr, true))
		{
			S2DE_FATAL_ERROR("Can't resize buffers");
			return false;
		}

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

		auto& io = ImGui::GetIO(); 
		io.ConfigFlags = Core::Engine::isEditor() ? ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NoMouseCursorChange
			: ImGuiConfigFlags_NoMouseCursorChange;

		// TODO: Why we are load font in renderer ?
		const auto resourceManager = Core::Resources::ResourceManager::GetInstance();
		Verify(resourceManager->Load<Render::Font>("default", Core::Resources::ResourceManager::DefaultResourceName, true), "Can't load default font");

		const auto defaultFont = resourceManager->Get<Render::Font>();
		if (defaultFont->Create(48.0f))
		{
			io.Fonts->AddFontFromMemoryTTF(defaultFont->GetTTFData(), 1, 24);
		}
		

		//LoadCustomImguiTheme();

		// Setup Platform / Renderer backends
		ImGui_ImplSDL2_InitForD3D(Core::GameWindow::GetInstance()->GetSDLWindow());
		ImGui_ImplDX11_Init(m_base.Device, m_base.Context);

		return true;
	}

	bool Renderer::CreateDebugLayer()
	{
		const auto hDll = LoadLibraryExA("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

		Verify(hDll != nullptr, "Couldn't load dxgidebug.dll!");
		Verify_HR(m_base.Device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_debug.DebugCore)), "Can't get ID3D11Debug interface!");

		typedef HRESULT (__stdcall * fPtr)(const IID&, void**);

		const auto DXGIGetDebugInterface = reinterpret_cast<fPtr>(GetProcAddress(hDll, "DXGIGetDebugInterface"));

		/*
			IDXGIDebug* debugDev = nullptr;
			DXGIGetDebugInterface(__uuidof(IDXGIDebug), reinterpret_cast<void**>(&debugDev));
			debugDev->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		*/

		Verify_HR(m_base.Device->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&m_debug.InfoQueue)), "Can't get ID3D11InfoQueue interface!");

		// Add debug breaks 
		m_debug.InfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
		m_debug.InfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);

		D3D11_MESSAGE_ID hide[] =
		{
			D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
		};
		
		D3D11_INFO_QUEUE_FILTER filter = { };
		filter.DenyList.NumIDs = _countof(hide);
		filter.DenyList.pIDList = hide;

		m_debug.InfoQueue->AddStorageFilterEntries(&filter);

		CaptureMessages();

		return true;
	}

	bool Renderer::CreateRenderTarget()
	{
		m_base.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_zBuffer.BackBuffer);
		Verify_HR(m_base.Device->CreateRenderTargetView(m_zBuffer.BackBuffer, nullptr, &m_base.RenderTarget), "Cannot create render target view");
		CreateFramebufferTexture(m_zBuffer.BackBuffer);

		if (m_framebuffer.BufferData != nullptr)
		{
			Verify_HR(m_base.Device->CreateRenderTargetView(m_framebuffer.BufferData, nullptr, &m_framebuffer.RenderTarget), "Cannot create render target for framebuffer!");
		}

		return true;
	}

	bool Renderer::CreateDeviceAndSwapChain()
	{
		const D3D_FEATURE_LEVEL featureLevels[] = 
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		const D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
			D3D_DRIVER_TYPE_UNKNOWN,
		};

		// Add debug device flag if debug render mode is defined...
#if defined(S2DE_DEBUG_RENDER_MODE)
		m_base.DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// Try to create device with correct driver type 
		HRESULT device_hr = S_OK;
		const auto driver_types_size = ARRAYSIZE(driverTypes);
		for (std::uint32_t driverTypeIndex = 0; driverTypeIndex < driver_types_size; driverTypeIndex++)
		{
			const auto driverType = driverTypes[driverTypeIndex];

			device_hr = D3D11CreateDevice(nullptr, driverType, nullptr, m_base.DeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &m_base.Device, nullptr, &m_base.Context);

			if (SUCCEEDED(device_hr))
			{
				break;
			}
		}
		
		Verify(m_base.Device != nullptr, "Can't create device and swap chain!");

		IDXGIDevice* dxgiDevice = nullptr;
		Verify_HR(m_base.Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice), "Can't get DXGI Device...");

		IDXGIAdapter* dxgiAdapter = nullptr;
		Verify_HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter), "Can't get DXGI Adapter...");

		IDXGIFactory* dxgiFactory = nullptr;
		Verify_HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory), "Can't get DXGI Factory...");

		const auto window = Core::GameWindow::GetInstance();
		DXGI_SWAP_CHAIN_DESC sd = { };

		sd.BufferDesc.Width  = window->GetWidth();
		sd.BufferDesc.Height = window->GetHeight();
		sd.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.RefreshRate.Numerator = m_params.Vsync == true ? 60 : 0;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		sd.BufferCount = 2;
		sd.Windowed = !window->isFullscreen();
		sd.OutputWindow = window->GetHWND();
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;

		Verify_HR(dxgiFactory->CreateSwapChain(m_base.Device, &sd, &m_base.SwapChain),
			"Cannot create swap chain...");

		// Release because it's not longer needed...
		Core::Release(dxgiDevice);
		Core::Release(dxgiAdapter);
		Core::Release(dxgiFactory);

		return true;
	}

	bool Renderer::CreateBlendState()
	{
		D3D11_BLEND_DESC bd = { };

		// TODO: Make diffrent modes
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX; // D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA; //D3D11_BLEND_ONE;

		Verify_HR(m_base.Device->CreateBlendState(&bd, &m_blend.BlendState[0]), "Can't create blend state on");

		bd.RenderTarget[0].BlendEnable = false;
		Verify_HR(m_base.Device->CreateBlendState(&bd, &m_blend.BlendState[1]), "Can't create blend state off");

		return true;
	}

	void Renderer::TurnOffAlphaBlending()
	{
		const static float factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_base.Context->OMSetBlendState(m_blend.BlendState[1], factor, 0xffffffff);
	}

	void Renderer::TurnOnAlphaBlending()
	{
		const static float factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_base.Context->OMSetBlendState(m_blend.BlendState[0], factor, 0xffffffff);
	}

	void Renderer::SetRasterizerState(Api::RasterizerMode mode)
	{
		m_base.Context->RSSetState(m_base.Rasterizer[mode]);
	}

	bool Renderer::CreateRasterizerStates()
	{
		D3D11_RASTERIZER_DESC defaultRasterDesc =
		{
			/* FillMode 				*/	Api::FillMode::cast(Api::FillMode::Type::Solid),
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



		// TODO:
		// Объединять enumы 
		// Чтобы добавлять то что я хочу и создать уникальный стейт

		ID3D11RasterizerState* rasterizer = nullptr;
		
		// Create the default rasterizer state 		
		Verify_HR(m_base.Device->CreateRasterizerState(&defaultRasterDesc, &rasterizer), "Render Error: Cannot create rasterizer state");
		m_base.Rasterizer.insert({ Api::RasterizerMode::Default, rasterizer });
		// Create the without culling rasterizer state 		
		auto nocullDesc = defaultRasterDesc;
		nocullDesc.CullMode = D3D11_CULL_NONE;
		Verify_HR(m_base.Device->CreateRasterizerState(&nocullDesc, &rasterizer), "Render Error: Cannot create rasterizer state");
		m_base.Rasterizer.insert({ Api::RasterizerMode::Default, rasterizer });

		// Create with FCC rasterizer state 		
		auto fccDesc = defaultRasterDesc;
		fccDesc.FrontCounterClockwise = true;
		Verify_HR(m_base.Device->CreateRasterizerState(&fccDesc, &rasterizer), "Render Error: Cannot create rasterizer state");
		m_base.Rasterizer.insert({ Api::RasterizerMode::TwoSided, rasterizer });



		return true;
	}

	bool Renderer::CreateDepthStencil()
	{
		if (!CreateRenderTarget())
			return false;

		const auto window = Core::GameWindow::GetInstance();
		
		// Set up the description of the depth buffer.
		D3D11_TEXTURE2D_DESC depthBufferDesc = { };
		depthBufferDesc.Width = window->GetWidth();
		depthBufferDesc.Height = window->GetHeight();
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		Verify_HR(m_base.Device->CreateTexture2D(&depthBufferDesc, NULL, &m_zBuffer.DepthStencilBuffer), "Render Error: Cannot create depth buffer");

		// Set up the description of the stencil state.
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = { };

		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		
		Verify_HR(m_base.Device->CreateDepthStencilState(&depthStencilDesc, &m_zBuffer.DepthState[0]), "Render Error: Cannot create enabled depth stencil state");

		depthStencilDesc.DepthEnable = false;
		Verify_HR(m_base.Device->CreateDepthStencilState(&depthStencilDesc, &m_zBuffer.DepthState[1]), "Render Error: Cannot create disabled depth stencil state");

		// Set up the depth stencil view description.
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = { };
		depthStencilViewDesc.Format = depthBufferDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		Verify_HR(m_base.Device->CreateDepthStencilView(m_zBuffer.DepthStencilBuffer, &depthStencilViewDesc, &m_zBuffer.DepthStencilView), "Render Error: Cannot create depth stencil view");

		// Set render target
		m_base.Context->OMSetRenderTargets(1, &m_base.RenderTarget, m_zBuffer.DepthStencilView);
		return true;	
	}

	void Renderer::UpdateViewport()
	{
		const auto window = Core::GameWindow::GetInstance();

		m_base.Viewport.Width = static_cast<float>(window->GetWidth());
		m_base.Viewport.Height = static_cast<float>(window->GetHeight());

		if (Core::Engine::isEditor() && m_params.ShowImguiWindows)
		{
			const auto renderWindow = GetImGui_Window<Editor::EditorRenderWindow*>("EditorRenderWindow");
			if (renderWindow != nullptr)
			{
				m_base.Viewport.Width = static_cast<float>(renderWindow->GetWindowWidth());
				m_base.Viewport.Height = static_cast<float>(renderWindow->GetWindowHeight());
			}
		}

		m_base.Viewport.MinDepth = 0.0f;
		m_base.Viewport.MaxDepth = 1.0f;
		m_base.Viewport.TopLeftX = 0.0f;
		m_base.Viewport.TopLeftY = 0.0f;
		
		m_base.Context->RSSetViewports(1, &m_base.Viewport);
	}

	void Renderer::Destroy()
	{
		Core::Utils::Logger::Log("[Renderer] Destroy...");

		FBX_Importer::GetInstance()->Destroy();

		if (Core::Engine::isEditor())
		{
			Core::Delete(m_editorToolStrip);
			Core::Delete(m_editorCenterCursor);
		}

		m_windowsStorage.clear();
		m_windowsStorage.shrink_to_fit();

		DestroyImGui();

		m_base.Rasterizer.clear();
		
		Core::Release(m_base.SwapChain);
		Core::Release(m_base.Device);
		Core::Release(m_base.Context);
		Core::Release(m_base.RenderTarget);
		Core::Release(m_framebuffer.RenderTarget);
		Core::Release(m_zBuffer.DepthStencilBuffer);
		Core::Release(m_zBuffer.DepthState[0]);
		Core::Release(m_zBuffer.DepthState[1]);
		Core::Release(m_zBuffer.DepthStencilView);
		Core::Release(m_framebuffer.BufferData);
		Core::Release(m_framebuffer.BufferSRV);

#if defined(S2DE_DEBUG_RENDER_MODE)
		m_debug.DebugCore->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
		Core::Release(m_debug.InfoQueue);
		Core::Release(m_debug.DebugCore);
#endif

	}

	void Renderer::Clear()
	{
		const float color_array[4] = { m_params.ClearColor.r, m_params.ClearColor.g, m_params.ClearColor.b , 1 };

		m_base.Context->OMSetRenderTargets(1, &m_base.RenderTarget, m_zBuffer.DepthStencilView);
		m_base.Context->OMSetDepthStencilState(m_zBuffer.DepthState[0], 0);

		m_base.Context->ClearRenderTargetView(m_base.RenderTarget, color_array);
		m_base.Context->ClearDepthStencilView(m_zBuffer.DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void Renderer::End()
	{
		m_base.SwapChain->Present(static_cast<std::uint32_t>(m_params.Vsync), 0);
	}

	void Renderer::DestroyImGui()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void Renderer::RenderImGui()
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void Renderer::UpdateImGuiWindows()
	{
		S2DE_IMGUI_NEW_FRAME();

		if (Core::Engine::isEditor())
		{
			const auto window = Core::GameWindow::GetInstance();

			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window->GetWidth()),
				static_cast<float>(window->GetHeight())));
			const auto windowFlags = ImGuiWindowFlags_NoBringToFrontOnFocus |
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

			const auto show = ImGui::Begin("Dockspace", NULL, windowFlags);

			ImGui::PopStyleVar();
			ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);	

			if (m_params.ShowImguiWindows)
			{
				for (const auto& it : m_windowsStorage)
				{
					it.second->Render(this);
				}
			}
			m_editorToolStrip->SetDrawState(show);
			m_editorToolStrip->Render(this);



			ImGui::End();
			
		}
		else
		{
			for (const auto& it : m_windowsStorage)
			{
				it.second->Render(this);
			}
		}

		#if defined(S2DE_DEBUG_RENDER_MODE)
				if (m_params.ShowImguiDemoWindow)
					ImGui::ShowDemoWindow(&m_params.ShowImguiDemoWindow);
		#endif
		
		ImGui::Render();
	}

	void Renderer::Render()
	{
		UpdateImGuiWindows();

#if defined(S2DE_DEBUG_RENDER_MODE)
		CaptureMessages();
#endif

		Clear();
		{
			// TODO: We need array of targets to make more render windows

			UpdateFramebufferShaderResource();

			const auto engine = Core::Engine::GetInstance();
			const auto sceneManager = Scene::SceneManager::GetInstance();
			if (Core::Engine::isEditor() && m_params.ShowImguiWindows)
			{
				// Basic support of render window

				if (m_framebuffer.RenderTarget != nullptr)
				{
					m_base.Context->OMSetRenderTargets(1, &m_framebuffer.RenderTarget, m_zBuffer.DepthStencilView);

					sceneManager->RenderScene(this);
					engine->GetApplicationHandle()->OnRender(this);
				}

				m_base.Context->OMSetRenderTargets(1, &m_base.RenderTarget, m_zBuffer.DepthStencilView);

				const auto renderWindow = GetImGui_Window<Editor::EditorRenderWindow*>("EditorRenderWindow");
				if (renderWindow != nullptr && m_framebuffer.BufferSRV != nullptr)
				{
					renderWindow->PushRenderTexture(m_framebuffer.BufferSRV);
				}

				const auto colorPicker = GetImGui_Window<Editor::EditorColorPicker*>("EditorBgColorPicker");
				if (colorPicker != nullptr)
				{
					colorPicker->SetColor(m_params.ClearColor);
				}
			}
			else
			{
				sceneManager->RenderScene(this);
				engine->GetApplicationHandle()->OnRender(this);
			}


			RenderImGui();

			
		}

		End();

	}

	ImGui_Window* Renderer::AddImGuiWindow(std::string name, ImGui_Window* wnd, bool visible)
	{
		if (Core::Utils::isStringEmpty(name) || wnd == nullptr)
		{
			Core::Utils::Logger::Error("[Renderer] Can't add window to storage!");
			return nullptr;
		}

		wnd->SetName(name);
		wnd->SetDrawState(visible);

		m_windowsStorage.push_back(std::make_pair(name, wnd));
		Core::Utils::Logger::Log("[Renderer] Added window %s", name.c_str());
		return wnd;
	}

	void Renderer::DeleteImGuiWindow(std::string name)
	{
		std::vector<std::pair<std::string, ImGui_Window*>>::const_iterator it = std::find_if(m_windowsStorage.begin(),
			m_windowsStorage.end(), [&name](std::pair<std::string, ImGui_Window*> const& elem) {
				return elem.first == name;
			});
		
		m_windowsStorage.erase(it, m_windowsStorage.end());
		Core::Utils::Logger::Log("[Renderer] Removed window %s", name.c_str());
	}

	void Renderer::SetBackColor(Math::Color<float> color)
	{
		m_params.ClearColor = color;
	}

	bool Renderer::CreateFramebufferTexture(ID3D11Texture2D* sw_buff)
	{
		D3D11_TEXTURE2D_DESC td = { };
		sw_buff->GetDesc(&td);

		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		Verify_HR(m_base.Device->CreateTexture2D(&td, NULL, &m_framebuffer.BufferData), "Can't create framebuffer texture data");

		const auto renderWindow = GetImGui_Window<Editor::EditorRenderWindow*>("EditorRenderWindow");
		if(renderWindow != nullptr)
		{
			renderWindow->Reset();
		}

		Core::Release(m_framebuffer.BufferSRV);

		return true;
	}

	void Renderer::UpdateFramebufferShaderResource()
	{
		ID3D11Texture2D* pBuffer = nullptr;
		m_base.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);

		if (pBuffer == nullptr)
		{
			CreateFramebufferTexture(m_framebuffer.BufferData);
			return;
		}
		
		m_base.Context->CopyResource(m_framebuffer.BufferData, pBuffer);
		Core::Release(pBuffer);

		if (m_framebuffer.BufferData == nullptr)
		{
			Core::Utils::Logger::Warning("[Renderer] Error: Framebuffer data is null!");
			return;
		}

		if (m_framebuffer.BufferSRV == nullptr)
		{
			D3D11_TEXTURE2D_DESC textureDesc = { };
			m_framebuffer.BufferData->GetDesc(&textureDesc);

			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = { };
			shaderResourceViewDesc.Format = textureDesc.Format;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;

			m_base.Device->CreateShaderResourceView(m_framebuffer.BufferData, &shaderResourceViewDesc, &m_framebuffer.BufferSRV);
		}
	}

	void Renderer::SetVsync(bool vsync)
	{
		m_params.Vsync = vsync;
	}

	bool Renderer::CaptureMessages()
	{
		const auto message_count = m_debug.InfoQueue->GetNumStoredMessages();

		for (std::int64_t i = 0; i < message_count; i++)
		{
			std::uint64_t message_size = 0;

			m_debug.InfoQueue->GetMessageA(i, nullptr, &message_size);

			D3D11_MESSAGE* message = reinterpret_cast<D3D11_MESSAGE*>(malloc(message_size));
			if (message == nullptr)
			{
				return false;
			}

			Verify_HR(m_debug.InfoQueue->GetMessageA(i, message, &message_size), "Can't get message from queue!");

			switch (message->Severity)
			{
				case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_ERROR:
					Core::Utils::Logger::Error("D3D11 ERROR %.*s", message->DescriptionByteLength, message->pDescription);
					break;
				case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_CORRUPTION:
					Core::Utils::Logger::Error("D3D11 CORRUPTION %.*s", message->DescriptionByteLength, message->pDescription);
					break;
				case D3D11_MESSAGE_SEVERITY::D3D11_MESSAGE_SEVERITY_WARNING:
					Core::Utils::Logger::Warning("D3D11 WARNING %.*s", message->DescriptionByteLength, message->pDescription);
					break;
				default:
					Core::Utils::Logger::LogColored(DirectX::SimpleMath::Color(0.5f, 0, 0.2f, 1),"D3D11 Info %.*s", message->DescriptionByteLength, message->pDescription);
					break;
			}

			delete message;
		}

		m_debug.InfoQueue->ClearStoredMessages();

		return true;
	}

	void Renderer::ToggleImGuiDemoWindowVisible()
	{
		m_params.ShowImguiDemoWindow =!m_params.ShowImguiDemoWindow;
	}

	void Renderer::ToggleImGuiWindowsVisible()
	{
		m_params.ShowImguiWindows =!m_params.ShowImguiWindows;

		if (Core::Engine::isEditor())
		{
			UpdateViewport();
		}
	}

	void Renderer::TurnZBufferOn()
	{
		m_base.Context->OMSetDepthStencilState(m_zBuffer.DepthState[0], 1);
	}

	void Renderer::TurnZBufferOff()
	{
		m_base.Context->OMSetDepthStencilState(m_zBuffer.DepthState[1], 1);
	}

	void Renderer::DrawIndexed(std::uint64_t indexCount, std::uint32_t startIndexLocation, std::uint32_t baseVertexLocation, D3D11_PRIMITIVE_TOPOLOGY topology)
	{
		m_base.Context->IASetPrimitiveTopology(topology);
		m_base.Context->DrawIndexed(static_cast<std::uint32_t>(indexCount), startIndexLocation, baseVertexLocation);
	}

	void Renderer::Draw(std::uint64_t vertexCount, std::uint32_t startVertexLocation, D3D11_PRIMITIVE_TOPOLOGY topology)
	{
		m_base.Context->IASetPrimitiveTopology(topology);
		m_base.Context->Draw(static_cast<std::uint32_t>(vertexCount), startVertexLocation);
	}

	inline ID3D11Device* Renderer::GetDevice()
	{
		return m_base.Device;
	}

	inline ID3D11DeviceContext* Renderer::GetContext()
	{
		return m_base.Context;
	}

	inline IDXGISwapChain* Renderer::GetSwapChain()
	{
		return m_base.SwapChain;
	}

	inline ID3D11RenderTargetView* Renderer::GetRenderTargetView()
	{
		return m_base.RenderTarget;
	}

	inline D3D11_VIEWPORT Renderer::GetViewport() const
	{
		return m_base.Viewport;
	}

	inline ID3D11Texture2D* Renderer::GetDepthStencilBuffer()
	{
		return m_zBuffer.DepthStencilBuffer;
	}

	inline ID3D11DepthStencilView* Renderer::GetDepthStencilView()
	{
		return m_zBuffer.DepthStencilView;
	}

	inline ID3D11ShaderResourceView* Renderer::GetFramebufferShaderResource() const
	{
		return m_framebuffer.BufferSRV;
	}

	inline ID3D11Texture2D* Renderer::GetFramebufferTextureData() const
	{
		return m_framebuffer.BufferData;
	}

	inline bool Renderer::GetVsync() const
	{
		return m_params.Vsync;
	}

	void Renderer::DebugDrawCube(Math::float3 pos,
		Math::float3 rot,
		Math::float3 scale,
		DirectX::SimpleMath::Color color)
	{
		VertexBuffer<Vertex>* vertexBuffer = nullptr;

		if (vertexBuffer == nullptr)
		{
			vertexBuffer = new VertexBuffer<Vertex>();

			vertexBuffer->GetArray() =
			{
				{ { -1.f, -1.f, -1.f },  { color.x,color.y,color.z,color.w }  },
				{ {  1.f, -1.f, -1.f },  { color.x,color.y,color.z,color.w }  },
				{ {  1.f, -1.f,  1.f },  { color.x,color.y,color.z,color.w }  },
				{ { -1.f, -1.f,  1.f },  { color.x,color.y,color.z,color.w }  },
				{ { -1.f,  1.f, -1.f },  { color.x,color.y,color.z,color.w }  },
				{ {  1.f,  1.f, -1.f },  { color.x,color.y,color.z,color.w }  },
				{ {  1.f,  1.f,  1.f },  { color.x,color.y,color.z,color.w }  },
				{ { -1.f,  1.f,  1.f },  { color.x,color.y,color.z,color.w }  }
			};

			Assert(vertexBuffer->Create(), "Can't create vertex buffer for debug cube");
			vertexBuffer->Update(this);
		}

		IndexBuffer<std::int32_t>* indexBuffer = nullptr;

		if (indexBuffer == nullptr)
		{
			indexBuffer = new IndexBuffer<std::int32_t>();

			indexBuffer->GetArray() =
			{
				0, 1,
				1, 2,
				2, 3,
				3, 0,
				4, 5,
				5, 6,
				6, 7,
				7, 4,
				0, 4,
				1, 5,
				2, 6,
				3, 7
			};

			Assert(indexBuffer->Create(), "Can't create index buffer for debug cube");
			indexBuffer->Update(this);
		}

		GameObjects::Components::Transform* transform = nullptr;
		if (transform == nullptr)
		{
			transform = new GameObjects::Components::Transform();

			transform->SetPosition(pos);
			transform->SetRotation(rot);
			transform->SetScale(scale);
		}

		vertexBuffer->Bind(this);
		indexBuffer->Bind(this);

		Shader* shader = Core::Resources::ResourceManager::GetInstance()->Get<Shader>("Line");

		shader->UpdateMainConstBuffer(this, transform->UpdateTransformation());
		shader->Bind(this);

		DrawIndexed(indexBuffer->GetArray().size(), 0, 0, D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		Core::Delete(vertexBuffer);
		Core::Delete(transform);
		Core::Delete(indexBuffer);
	}

	void Renderer::DebugDrawRing(Math::float3 pos,
		Math::float3 majorAxis,
		Math::float3 minorAxis,
		DirectX::SimpleMath::Color color)
	{
		VertexBuffer<Vertex>* vertexBuffer = nullptr;

		if (vertexBuffer == nullptr)
		{
			vertexBuffer = new VertexBuffer<Vertex>();

			static const std::int8_t ringSegments = 32;
			static const Math::float4 initialCos = { 1.f, 1.f, 1.f, 1.f };

			float angleDelta = DirectX::XM_2PI / float(ringSegments);

			Math::float4 cosDelta = DirectX::XMVectorReplicate(std::cos(angleDelta));
			Math::float4 sinDelta = DirectX::XMVectorReplicate(std::sin(angleDelta));
			Math::float4 incrementalSin = DirectX::XMVectorZero();

			Math::float4 incrementalCos = initialCos;
			for (std::uint32_t i = 0; i < ringSegments; i++)
			{
				Math::float3  _pos = DirectX::XMVectorMultiplyAdd(majorAxis, incrementalCos, {0,0,0});
				_pos = DirectX::XMVectorMultiplyAdd(minorAxis, incrementalSin, _pos);
				vertexBuffer->GetArray().push_back({ _pos , color });

				// Standard formula to rotate a vector.
				Math::float4 newCos = incrementalCos * cosDelta - incrementalSin * sinDelta;
				Math::float4 newSin = incrementalCos * sinDelta + incrementalSin * cosDelta;
				incrementalCos = newCos;
				incrementalSin = newSin;
			}

			vertexBuffer->GetArray().push_back(vertexBuffer->GetArray()[0]);
			Assert(vertexBuffer->Create(), "Can't create index buffer for ring line");
			vertexBuffer->Update(this);
		}

		Shader* shader = Core::Resources::ResourceManager::GetInstance()->Get<Shader>("Line");
		GameObjects::Components::Transform* transform = nullptr;

		if (transform == nullptr)
		{
			transform = new GameObjects::Components::Transform();
			transform->SetPosition(pos);
		}

		vertexBuffer->Bind(this);
		shader->UpdateMainConstBuffer(this, transform->UpdateTransformation());
		shader->Bind(this);

		Draw(vertexBuffer->GetArray().size(), 0, D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		
		Core::Delete(vertexBuffer);
		Core::Delete(transform);
	}

	void Renderer::DebugDrawSphere(Math::float3 pos,
			float radius,
			DirectX::SimpleMath::Color color)
	{
		const Math::float3 xaxis = DirectX::g_XMIdentityR0 * radius;
		const Math::float3 yaxis = DirectX::g_XMIdentityR1 * radius;
		const Math::float3 zaxis = DirectX::g_XMIdentityR2 * radius;

		DebugDrawRing(pos, xaxis, zaxis, color);
		DebugDrawRing(pos, xaxis, yaxis, color);
		DebugDrawRing(pos, yaxis, zaxis, color);
	}

	void Renderer::DebugDrawLineCross(Math::float3 pos,
		Math::float3 rot,
		Math::float3 scale,
		DirectX::SimpleMath::Color color)
	{
		VertexBuffer<Vertex>* vertexBuffer = nullptr;
		Shader* shader = Core::Resources::ResourceManager::GetInstance()->Get<Shader>("Line");

		if (vertexBuffer == nullptr)
		{
			vertexBuffer = new VertexBuffer<Vertex>();

			vertexBuffer->GetArray() =
			{
				{ { -1.0f, -1.0f,   0.0f },    color },
				{ { 0.0f,   0.0f,	0.0f }, color },
				{ { 1.0f,   -1.0f,	0.0f }, color },
				{ { -1.0f,   1.0f,	0.0f }, color },
				{ { 0.0f,   0.0f,	0.0f }, color },
				{ { 1.0f,   1.0f,	0.0f }, color },
			};

			Assert(vertexBuffer->Create(), "Can't create index buffer for debug line");
			vertexBuffer->Update(this);
		}

		GameObjects::Components::Transform* transform = nullptr;

		if (transform == nullptr)
		{
			transform = new GameObjects::Components::Transform();

			transform->SetPosition(pos);
			transform->SetRotation(rot);
			transform->SetScale(scale);
		}

		vertexBuffer->Bind(this);
		shader->UpdateMainConstBuffer(this, transform->UpdateTransformation());
		shader->Bind(this);

		Draw(vertexBuffer->GetArray().size(), 0, D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		Core::Delete(vertexBuffer);
		Core::Delete(transform);

	}

	void Renderer::DebugDrawLine(Math::float3 begin,
		Math::float3 end,
		DirectX::SimpleMath::Color color)
	{
		VertexBuffer<Vertex>* vertexBuffer = nullptr;
		Shader* shader = Core::Resources::ResourceManager::GetInstance()->Get<Shader>("Line");

		if (vertexBuffer == nullptr)
		{
			vertexBuffer = new VertexBuffer<Vertex>();

			vertexBuffer->GetArray() =
			{
				{ begin, { color.x,color.y,color.z,color.w } },
				{ end,	 { color.x,color.y,color.z,color.w } },
			};

			Assert(vertexBuffer->Create(), "Can't create index buffer for debug line");
			vertexBuffer->Update(this);
		}

		vertexBuffer->Bind(this);
		shader->Bind(this);

		Draw(vertexBuffer->GetArray().size(), 0, D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		Core::Delete(vertexBuffer);
	}
}