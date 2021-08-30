#include "Renderer.h"
#include "Base/Engine.h"

#define S2DE_FORMAT_MODE DXGI_FORMAT_R8G8B8A8_UNORM

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
							m_vsync(true)
	{

	}


	Renderer::~Renderer()
	{

	}

	bool Renderer::Reset()
	{
		UpdateViewport();
		return true;
	}

	bool Renderer::GetDisplay(std::uint32_t& mode_numerator, std::uint32_t& mode_denominator)
	{
		IDXGIFactory* factory = nullptr;
		IDXGIAdapter* adapter = nullptr;
		IDXGIOutput* adapterOutput = nullptr;

		//Create the dxgi factory
		if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
		{
			S2DE_FATAL_ERROR("Cannot create DXGI factory");
			return false;
		}

		if (FAILED(factory->EnumAdapters(0, &adapter)))
		{
			S2DE_FATAL_ERROR("Cannot get adapters for this video card");
			return false;
		}

		//Enumerate the primary adapter output (monitor).;
		if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
		{
			S2DE_FATAL_ERROR("Cannot get primary adapter output");
			return false;
		}

		//Get the number of modes 
		std::uint32_t  numModes = 0;

		if (FAILED(adapterOutput->GetDisplayModeList(S2DE_FORMAT_MODE, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL)))
		{
			S2DE_FATAL_ERROR("Cannot get number of modes");
			return false;
		}

		//Create a list to hold all the possible display modes for this monitor/video card combination.
		DXGI_MODE_DESC* display_mode_list = new DXGI_MODE_DESC[numModes];


		if (FAILED(adapterOutput->GetDisplayModeList(S2DE_FORMAT_MODE, DXGI_ENUM_MODES_INTERLACED, &numModes, display_mode_list)))
		{
			S2DE_FATAL_ERROR("Cannot get display mods");
			return false;
		}

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

		if (FAILED(adapter->GetDesc(&adapterDesc)))
		{
			S2DE_FATAL_ERROR("Cannot get adapter description");
			return false;
		}

		//Store the dedicated video card memory in megabytes.
		m_videocard_desc.Memory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	
		//Convert the name of the video card to a character array and store it.
		if (wcstombs_s(0, m_videocard_desc.Description, 128, adapterDesc.Description, 128)
			!= 0)
		{
			S2DE_FATAL_ERROR("Cannot set name of video card");
			return false;
		}
		
		Logger::Log("Video card %s Mem = %d initialized!", m_videocard_desc.Description, m_videocard_desc.Memory);	

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
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
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



		m_feature_level = D3D_FEATURE_LEVEL_11_0;

		//TODO
		//Flags
		if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &m_feature_level, 1,
			D3D11_SDK_VERSION, &swap_chain_desc, &m_swapChain, &m_device, NULL, &m_deviceContext)))
		{
			S2DE_FATAL_ERROR("Cannot create device and swap chain");
			return false;
		}

		//Create the render target view with the back buffer pointer.
		ID3D11Texture2D* backBufferPtr;
		m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		if (FAILED(m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView)))
		{
			S2DE_FATAL_ERROR("Cannot create render target view");
			return false;
		}

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

		if (FAILED(m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer)))
		{
			S2DE_FATAL_ERROR("Cannot create depth buffer");
			return false;
		}

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
		if (FAILED(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState)))
		{
			S2DE_FATAL_ERROR("Cannot create depth stencil state");
			return false;
		}

		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		//Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		//Create the depth stencil view.
		if (FAILED(m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView)))
		{
			S2DE_FATAL_ERROR("Cannot create depth stencil view");
			return false;
		}

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

		if (FAILED(m_device->CreateRasterizerState(&rasterDesc, &m_rasterState)))
		{
			S2DE_FATAL_ERROR("Cannot create rasterizer state");
			return false;
		}

		//Now set the rasterizer state.
		m_deviceContext->RSSetState(m_rasterState);
		
		return true;	
	}

	void Renderer::UpdateViewport()
	{
		Logger::Log("<Renderer> UpdateViewport");

		m_viewport.Width = (float)Engine::GetGameWindow()->GetWidth();
		m_viewport.Height = (float)Engine::GetGameWindow()->GetHeight();
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;

		// Create the viewport.
		m_deviceContext->RSSetViewports(1, &m_viewport);
	}

	bool Renderer::Create()
	{
		Logger::Log("<Renderer> Get display");
		if (!GetDisplay(m_mode_numerator, m_mode_denominator))
			return false;

		Logger::Log("<Renderer> CreateDeviceAndSwapChain");
		if (!CreateDeviceAndSwapChain())
			return false;
		
		Logger::Log("<Renderer> CreateDepthStencil");
		if (!CreateDepthStencil())
			return false;

		return true;
	}

	void Renderer::Destroy()
	{
		Logger::Log("Destroying renderer...");

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
		m_deviceContext->ClearRenderTargetView(m_renderTargetView, m_clearColor);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void Renderer::End()
	{
		m_swapChain->Present((std::uint32_t)m_vsync, 0);
	}

	void Renderer::Render()
	{
		Clear();
		End();
	}
}