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

#define S2DE_IMGUI_NEW_FRAME()  ImGui_ImplDX11_NewFrame(); \
								ImGui_ImplWin32_NewFrame(); \
								ImGui::NewFrame(); \

#define S2DE_CONSOLE_RENDER() \
if (Engine::GetConsole() != nullptr) \
	Engine::GetConsole()->Render(); \

using namespace S2DE::Core;
using namespace S2DE::Core::Other;
using namespace S2DE::Core::Debug;
using namespace S2DE::Math;
using namespace S2DE::Editor;

namespace S2DE::Render
{
	//TODO 
	//Maybe need to move out most of desc's to header for quick var changes 


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
							m_device_flag(0),
							m_feature_level(D3D_FEATURE_LEVEL_11_0),
							m_render_fill_mode(RenderFillMode::Solid),
							m_clearColor(Color<float>(0.0f, 0.0f, 0.0f, 1.0f))

	{

	}


	Renderer::~Renderer()
	{

	}

	bool Renderer::Reset()
	{
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
		ImGui::GetStyle().WindowMinSize = ImVec2(300.0f, 400.0f);
		
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
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
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.39f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);

		colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);

		colors[ImGuiCol_Button] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);

		colors[ImGuiCol_Header] = ImVec4(0.14f, 0.14f, 0.14f, 0.55f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.34f, 0.24f, 0.24f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);

		colors[ImGuiCol_Separator] = ImVec4(0.7f, 0.7f, 0.7f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.24f, 0.24f, 0.24f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
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
		io.ConfigFlags = Engine::isEditor() ? ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NoMouseCursorChange : ImGuiConfigFlags_NoMouseCursorChange;

		//Search custom font
		std::string path;
		if (Engine::GetResourceManager().GetFilePath(S2DE_DEFAULT_FONT_NAME, "Font", ".ttf", path))
		{
			Logger::Log("Added custom font for imgui...");
			io.Fonts->AddFontFromFileTTF(path.c_str(), 19);
		}

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
		S2DE_CHECK(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL), "Render Error: Cannot get number of modes");

		//Create a list to hold all the possible display modes for this monitor/video card combination.
		DXGI_MODE_DESC* display_mode_list = new DXGI_MODE_DESC[numModes];
		S2DE_CHECK(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, display_mode_list), "Render Error: Cannot get display mods");

		for (std::uint32_t i = 0; i < numModes; i++)
		{
			if (display_mode_list[i].Width == Engine::GetGameWindow()->GetWidth())
			{
				if (display_mode_list[i].Height == Engine::GetGameWindow()->GetHeight())
				{
					//Logger::Log("format = %d w = %d h = %d refresh rate = %d scaling = %d scanline ordering = %d ", display_mode_list[i].Format, display_mode_list[i].Width, display_mode_list[i].Height, display_mode_list[i].RefreshRate, display_mode_list[i].Scaling, display_mode_list[i].ScanlineOrdering);
					mode_numerator = display_mode_list[i].RefreshRate.Numerator;
					mode_denominator = display_mode_list[i].RefreshRate.Denominator;
				}
			}
		}

		DXGI_ADAPTER_DESC adapterDesc;
		S2DE_CHECK(adapter->GetDesc(&adapterDesc), "Render Error: Cannot get adapter description");

		//Store the dedicated video card memory in megabytes.
		m_videocard_desc.Memory = (std::int32_t)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	
		//Convert the name of the video card to a character array and store it.
		S2DE_ASSERT(wcstombs_s(0, m_videocard_desc.Description, 128, adapterDesc.Description, 128) == 0);
		
		Logger::Log("[Renderer] Video card %s initialized! Memory = %d", m_videocard_desc.Description, m_videocard_desc.Memory);	

		Release(factory);
		Release(adapter);
		Release(adapterOutput);
		Delete(display_mode_list);

		return true;
	}

	bool Renderer::CreateDeviceAndSwapChain()
	{
		DXGI_SWAP_CHAIN_DESC swap_chain_desc;

		//Buffer mode desc init
		swap_chain_desc.BufferDesc.Width = Engine::GetGameWindow()->GetWidth();
		swap_chain_desc.BufferDesc.Height = Engine::GetGameWindow()->GetHeight();
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		//Swap chain desc init
		swap_chain_desc.BufferCount = 1;
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
		swap_chain_desc.Windowed = !Engine::GetGameWindow()->isFullscreen();
		swap_chain_desc.OutputWindow = Engine::GetGameWindow()->GetHWND();

		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
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

		//Reset buffers
		//without reseting buffers we get something like broken resolution 
		S2DE_CHECK(m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0), "Render Error: Can't reset buffers");

		//Create the render target view with the back buffer pointer.
		ID3D11Texture2D* backBufferPtr;

		m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		S2DE_CHECK(m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView), "Render Error: Cannot create render target view");
		CreateFramebufferTexture(backBufferPtr);
		Release(backBufferPtr);

		UpdateViewport();

		return true;
	}

	void Renderer::SwitchFillMode(RenderFillMode mode)
	{
		m_render_fill_mode = mode;
		CreateRasterizerState();
	}

	bool Renderer::CreateBlendState()
	{
		D3D11_BLEND_DESC blend_desc;
		ZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));

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

		D3D11_RASTERIZER_DESC rasterDesc;

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

		//Initialize the description of the stencil state.
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
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

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		//Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		//Create the depth stencil state.
		S2DE_CHECK(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState), "Render Error: Cannot create depth stencil state");
		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

		//Create the depth stencil view.
		S2DE_CHECK(m_device->CreateDepthStencilView(m_depthStencilBuffer, nullptr, &m_depthStencilView), "Render Error: Cannot create depth stencil view");
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
		Logger::Log("[Renderer] Get display...");
		if (!GetDisplay(m_mode_numerator, m_mode_denominator))
			return false;

		Logger::Log("[Renderer] Create device and swapchain...");
		if (!CreateDeviceAndSwapChain())
			return false;
		
		Logger::Log("[Renderer] Create depth stencil...");
		if (!CreateDepthStencil())
			return false;

		Logger::Log("[Renderer] Create rasterizer state...");
		if (!CreateRasterizerState())
			return false;

		Logger::Log("[Renderer] Create blend state...");
		if (!CreateBlendState())
			return false;

		Logger::Log("[Renderer] Initialize ImGui...");
		if (!InitImGui())
			return false;

		AddImGuiWindow("DebugInfoWindow", new Debug_Info());
		AddImGuiWindow("DebugObjectInspectorWindow", new Debug_ObjectInspector());

		if (Engine::isEditor())
		{
			//AddImGuiWindow("EditorRenderWindow", new EditorRenderWindow());
			//GetImGui_Window("EditorRenderWindow")->ToggleDraw();

			AddImGuiWindow("EditorToolStrip", new EditorToolStrip());
			GetImGui_Window("EditorToolStrip")->ToggleDraw();

			AddImGuiWindow("EditorObjectInspector", new EditorObjectInspector());
			GetImGui_Window("EditorObjectInspector")->ToggleDraw(); 
		}
		
		return true;
	}

	void Renderer::Destroy()
	{
		Logger::Log("[Renderer] Destroy...");

		m_windows_storage.clear();
		m_windows_storage.shrink_to_fit();

		DestroyImGui();

		Release(m_swapChain);
		Release(m_device);
		Release(m_deviceContext);
		Release(m_renderTargetView);
		Release(m_rasterState);
		Release(m_depthStencilBuffer);
		Release(m_depthStencilState);
		Release(m_depthStencilView);
		Release(m_framebuffer_data);
		Release(m_framebuffer_shaderResourceView);
	}

	void Renderer::Clear()
	{
		float color_array[4] = { m_clearColor.r / 1000, m_clearColor.g / 1000, m_clearColor.b / 1000, m_clearColor.a / 1000 };

		m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);
		m_deviceContext->ClearRenderTargetView(m_renderTargetView, color_array);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView,  D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
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

		for (std::vector<std::pair<std::string, ImGui_Window*>>::iterator it = m_windows_storage.begin(); it != m_windows_storage.end(); it++)
			it->second->Render();
		

		Engine::GetSceneManager()->RenderDebugGUI();

		ImGui::Render();
	}

	void Renderer::Render()
	{
		UpdateImGuiWindows();

		Clear();

		Engine::GetApplicationHandle()->OnRender();
		Engine::GetSceneManager()->RenderScene();
		UpdateFramebufferShaderResource();
		

		RenderImGui();

		if (Engine::isEditor())
		{
			if (m_framebuffer_shaderResourceView != nullptr && GetImGui_Window("EditorRenderWindow") != nullptr)
				reinterpret_cast<EditorRenderWindow*>(GetImGui_Window("EditorRenderWindow"))->PushRenderTexture((void*)m_framebuffer_shaderResourceView);
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
		D3D11_TEXTURE2D_DESC td;
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

		D3D11_TEXTURE2D_DESC textureDesc;
		m_framebuffer_data->GetDesc(&textureDesc);

		
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		m_device->CreateShaderResourceView(m_framebuffer_data, &shaderResourceViewDesc, &m_framebuffer_shaderResourceView);
	}
}