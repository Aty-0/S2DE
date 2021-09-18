#include "Renderer.h"
#include "Base/Engine.h"
#include "Base/DebugTools/VisualConsole.h"
#include "Base/ApplicationHandle.h"
#include "Base/GameWindow.h"

#include "GameObjects/Camera.h"

#define S2DE_FORMAT_MODE DXGI_FORMAT_R8G8B8A8_UNORM

#define S2DE_IMGUI_NEW_FRAME()  ImGui_ImplDX11_NewFrame(); \
								ImGui_ImplWin32_NewFrame(); \
								ImGui::NewFrame(); \

#define S2DE_CONSOLE_RENDER() \
if (Engine::GetConsole() != nullptr) \
	Engine::GetConsole()->Render(); \

namespace S2DE
{
	Renderer::Renderer() :	m_swapChain(nullptr),
							m_device(nullptr),
							m_deviceContext(nullptr),
							m_renderTargetView(nullptr),
							m_rasterState(nullptr),
							m_depthStencilBuffer(nullptr),
							m_depthStencilState(nullptr),
							m_depthStencilView(nullptr),
							m_vsync(true),
							m_device_flag(0),
							m_feature_level(D3D_FEATURE_LEVEL_11_0)
	{
		
	}


	Renderer::~Renderer()
	{

	}

	bool Renderer::Reset()
	{
		m_deviceContext->OMSetRenderTargets(0, 0, 0);
		Release(m_renderTargetView);

		
		if(FAILED(m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0)))
			return false;


		ID3D11Texture2D* pBuffer;
		S2DE_CHECK(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer), "Render Error: Can't create buffer");
		S2DE_CHECK(m_device->CreateRenderTargetView(pBuffer, NULL, &m_renderTargetView), "Render Error: Can't recreate render target view");

		Release(pBuffer);

		m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);


		UpdateViewport();
		return true;
	}

	void Renderer::LoadCustomImguiTheme()
	{
		ImGui::GetStyle().FrameRounding = 4.0f;
		ImGui::GetStyle().GrabRounding = 4.0f;
		ImGui::GetStyle().TabRounding = 4.0f;
		ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f);
		ImGui::GetStyle().WindowMinSize = ImVec2(300.0f, 400.0f);
		ImGui::GetStyle().WindowRounding = 6.0f;

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 0.75f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
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
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		LoadCustomImguiTheme();

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(Engine::GetGameWindow()->GetHWND());
		ImGui_ImplDX11_Init(m_device, m_deviceContext);

		return true;
	}

	bool Renderer::GetDisplay(std::uint32_t& mode_numerator, std::uint32_t& mode_denominator)
	{
		IDXGIFactory* factory = nullptr;
		IDXGIAdapter* adapter = nullptr;
		IDXGIOutput* adapterOutput = nullptr;

		//Create the dxgi factory
		S2DE_CHECK(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory), "Render Error: Cannot create DXGI factory");
		S2DE_CHECK(factory->EnumAdapters(0, &adapter), "Render Error: Cannot get adapters for this video card");


		//Enumerate the primary adapter output (monitor).;
		S2DE_CHECK(adapter->EnumOutputs(0, &adapterOutput), "Render Error: Cannot get primary adapter output");

		//Get the number of modes 
		std::uint32_t  numModes = 0;

		S2DE_CHECK(adapterOutput->GetDisplayModeList(S2DE_FORMAT_MODE, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL), "Render Error: Cannot get number of modes");

		//Create a list to hold all the possible display modes for this monitor/video card combination.
		DXGI_MODE_DESC* display_mode_list = new DXGI_MODE_DESC[numModes];
		S2DE_CHECK(adapterOutput->GetDisplayModeList(S2DE_FORMAT_MODE, DXGI_ENUM_MODES_INTERLACED, &numModes, display_mode_list), "Render Error: Cannot get display mods");


		for (std::uint32_t i = 0; i < numModes; i++)
		{
			if (display_mode_list[i].Width == Engine::GetGameWindow()->GetWidth())
			{
				if (display_mode_list[i].Height == Engine::GetGameWindow()->GetHeight())
				{
					mode_numerator = display_mode_list[i].RefreshRate.Numerator;
					mode_denominator = display_mode_list[i].RefreshRate.Denominator;
				}
			}
		}

		DXGI_ADAPTER_DESC adapterDesc;
		S2DE_CHECK(adapter->GetDesc(&adapterDesc), "Render Error: Cannot get adapter description");

		//Store the dedicated video card memory in megabytes.
		m_videocard_desc.Memory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	
		//Convert the name of the video card to a character array and store it.
		S2DE_CHECK(wcstombs_s(0, m_videocard_desc.Description, 128, adapterDesc.Description, 128)
			!= 0, "Render Error: Cannot set name of video card");
		
		Logger::Log("[Renderer] Video card %s Mem = %d initialized!", m_videocard_desc.Description, m_videocard_desc.Memory);	

		Release(factory);
		Release(adapter);
		Release(adapterOutput);
		Delete(display_mode_list);

		return true;
	}

	bool Renderer::CreateDeviceAndSwapChain()
	{
		DXGI_SWAP_CHAIN_DESC swap_chain_desc;
		swap_chain_desc.BufferCount = 1;
		swap_chain_desc.BufferDesc.Width = Engine::GetGameWindow()->GetWidth();
		swap_chain_desc.BufferDesc.Height = Engine::GetGameWindow()->GetHeight();
		swap_chain_desc.BufferDesc.Format = S2DE_FORMAT_MODE;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.OutputWindow = Engine::GetGameWindow()->GetHWND();

		//TODO
		//Global var
		//Turn multisampling off.
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
		swap_chain_desc.Windowed = !Engine::GetGameWindow()->isFullscreen();
		swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swap_chain_desc.SwapEffect  = DXGI_SWAP_EFFECT_DISCARD;
		swap_chain_desc.Flags = 0;

		if (m_vsync)
		{
			swap_chain_desc.BufferDesc.RefreshRate.Numerator = m_mode_numerator;
			swap_chain_desc.BufferDesc.RefreshRate.Denominator = m_mode_denominator;
		}
		else
		{
			swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
			swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
		}


		S2DE_CHECK(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, m_device_flag, &m_feature_level, 1,
			D3D11_SDK_VERSION, &swap_chain_desc, &m_swapChain, &m_device, NULL, &m_deviceContext), "Render Error: Cannot create device and swap chain");


		//Create the render target view with the back buffer pointer.
		ID3D11Texture2D* backBufferPtr;
		m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		S2DE_CHECK(m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView), "Render Error: Cannot create render target view");

		Release(backBufferPtr);

		return true;
	}

	bool Renderer::CreateDepthStencil()
	{
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		//Set up the description of the depth buffer.
		depthBufferDesc.Width = Engine::GetGameWindow()->GetWidth();
		depthBufferDesc.Height = Engine::GetGameWindow()->GetHeight();
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


		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

		//Initialize the description of the stencil state.
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		//Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

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

		//Create the depth stencil state.
		S2DE_CHECK(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState), "Render Error: Cannot create depth stencil state");

		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		//Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		//Create the depth stencil view.
		S2DE_CHECK(m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView), "Render Error: Cannot create depth stencil view");

		m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);

		D3D11_RASTERIZER_DESC rasterDesc;

		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		//Create the rasterizer state from the description we just filled out.
		S2DE_CHECK(m_device->CreateRasterizerState(&rasterDesc, &m_rasterState), "Render Error: Cannot create rasterizer state");


		//Now set the rasterizer state.
		m_deviceContext->RSSetState(m_rasterState);
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
		Logger::Log("[Renderer] Get display...");
		if (!GetDisplay(m_mode_numerator, m_mode_denominator))
			return false;

		Logger::Log("[Renderer] Create device and swapchain...");
		if (!CreateDeviceAndSwapChain())
			return false;
		
		Logger::Log("[Renderer] Create depth stencil...");
		if (!CreateDepthStencil())
			return false;

		Logger::Log("[Renderer] Initialize ImGui...");
		if (!InitImGui())
			return false;

		//FIX ME 
		//Temp fix
		Reset();

		Camera::_Camera = new Camera();
		return true;
	}

	void Renderer::Destroy()
	{
		Logger::Log("[Renderer] Destroy...");

		DestroyImGui();

		Release(m_swapChain);
		Release(m_device);
		Release(m_deviceContext);
		Release(m_renderTargetView);
		Release(m_rasterState);
		Release(m_depthStencilBuffer);
		Release(m_depthStencilState);
		Release(m_depthStencilView);
	}


	void Renderer::Clear()
	{
		m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);
		m_deviceContext->ClearRenderTargetView(m_renderTargetView, m_clearColor);
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
		S2DE_CONSOLE_RENDER();

		Camera::_Camera->DebugGUI();

		ImGui::Render();
	}

	void Renderer::Render()
	{
		UpdateImGuiWindows();

		Clear();

		Engine::GetApplicationHandle()->OnRender();
		Camera::_Camera->Update(0);

		RenderImGui();

		End();
	}
}