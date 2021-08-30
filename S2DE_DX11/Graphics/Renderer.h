#pragma once
#include "Base/Main/Common.h"

namespace S2DE
{
	class S2DE_DX11_API Renderer
	{
	public:
		Renderer();
		~Renderer();

		bool					 Create();
		bool					 Reset();
		void					 Destroy();
		void					 Render();

	private:					 
		bool					 GetDisplay(std::uint32_t& mode_numerator, std::uint32_t& mode_denominator);
		bool					 CreateDeviceAndSwapChain();
		bool					 CreateDepthStencil();
		void					 UpdateViewport();
		void				     Clear();
		void					 End();

		IDXGISwapChain*			 m_swapChain;
		ID3D11Device*			 m_device;
		ID3D11DeviceContext*	 m_deviceContext;
		ID3D11RenderTargetView*  m_renderTargetView;
		ID3D11RasterizerState*   m_rasterState;
		VideoCardDesc			 m_videocard_desc;

		//Depth strencil
		ID3D11Texture2D*		 m_depthStencilBuffer;
		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11DepthStencilView*  m_depthStencilView;

		bool					 m_vsync;
		D3D_FEATURE_LEVEL		 m_feature_level;
		std::uint32_t			 m_mode_numerator;
		std::uint32_t			 m_mode_denominator;

		D3D11_VIEWPORT			 m_viewport;

		//TODO
		//Own class
		float					m_clearColor[4] = { 0.7f, 0.5f, 1.0f, 1.0f };
	};
}