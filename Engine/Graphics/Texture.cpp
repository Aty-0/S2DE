#include "Texture.h"
#include "Base/Engine.h"
#include "Graphics/Renderer.h"

#include <D3DX11tex.h>

namespace S2DE::Render
{
	Texture::Texture() 
	{
		m_type = "Texture";
		m_ex = { ".dds", ".png", ".tga", ".jpg" };
	}

	Texture::~Texture()
	{

	}

	void Texture::Cleanup()
	{
		Core::Release(m_resource);
		Core::Release(m_texture_resource);
	}

	bool Texture::Load(std::string path)
	{
		if (Core::Other::isStringEmpty(path))
			return false;

		ID3D11Resource* res;

		S2DE_CHECK(D3DX11CreateShaderResourceViewFromFile(Core::Engine::GetRenderer()->GetDevice(), path.c_str(), NULL, NULL, &m_resource, NULL), "Can't create texture!");

		m_resource->GetResource(&res);
		res->QueryInterface<ID3D11Texture2D>(&m_texture_resource);

		m_texture_resource->GetDesc(&m_texture_desc);

		Core::Release(res);

		D3D11_SAMPLER_DESC samplerDesc;

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;

		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		samplerDesc.MipLODBias = 0.0f;

		samplerDesc.MaxAnisotropy = 0;

		S2DE_CHECK(Core::Engine::GetRenderer()->GetDevice()->CreateSamplerState(&samplerDesc, &m_texture_sampler_state), "Can't create sampler state");

		return true;
	}

	void Texture::Bind(std::uint32_t NumViews)
	{
		Core::Engine::GetRenderer()->GetContext()->PSSetShaderResources(0, NumViews, &m_resource);
		Core::Engine::GetRenderer()->GetContext()->PSGetSamplers(0, NumViews, &m_texture_sampler_state);
	}
}