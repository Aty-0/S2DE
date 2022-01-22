#include "Texture.h"
#include "Base/Engine.h"
#include "Graphics/Renderer.h"

#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

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
		Core::Release(m_resourceview);
		Core::Release(m_texture2d);
		Core::Release(m_texture_sampler_state);
	}

	void Texture::UpdateTextureDesc()
	{
		m_resourceview->GetResource(&m_resource);
		m_resource->QueryInterface<ID3D11Texture2D>(&m_texture2d);
		m_texture2d->GetDesc(&m_texture_desc);
	}

	bool Texture::Load(std::string path)
	{
		//If path is empty
		if (Core::Other::isStringEmpty(path))
		{
			Logger::Error("No path setted for texture!");
			CreateEmptyTexture();
			return false;
		}

		HRESULT hr;

		//If file extension is a dds we are use CreateDDSTextureFromFile function
		//if not we use CreateWICTextureFromFile
		if (Core::Other::GetFileExtension(path) == ".dds")
		{
			hr = DirectX::CreateDDSTextureFromFile(Core::Engine::GetRenderer()->GetDevice(), Core::Other::StringToWString(path).c_str(), &m_resource, &m_resourceview);
			if (FAILED(hr))
			{
				Logger::Error("Can't create dds texture from file %s %s", path.c_str(), Core::Utils::GetHRCodeDetails(hr));
				CreateEmptyTexture();
				return false;
			}
		}
		else
		{
			hr = DirectX::CreateWICTextureFromFile(Core::Engine::GetRenderer()->GetDevice(), Core::Other::StringToWString(path).c_str(), &m_resource, &m_resourceview);
			if (FAILED(hr))
			{
				Logger::Error("Can't create texture from file %s %s", path.c_str(), Core::Utils::GetHRCodeDetails(hr));
				CreateEmptyTexture();
				return false;
			}
		}
		
		//Set sampler state
		D3D11_SAMPLER_DESC desc{};

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.BorderColor[0] = 0;
		desc.BorderColor[1] = 0;
		desc.BorderColor[2] = 0;
		desc.BorderColor[3] = 0;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = FLT_MAX;
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;

		S2DE_CHECK(Core::Engine::GetRenderer()->GetDevice()->CreateSamplerState(&desc, &m_texture_sampler_state), "Can't create sampler state");

		UpdateTextureDesc();
		return true;
	}

	bool Texture::CreateEmptyTexture()
	{
		//TODO
		//Basic custom color change
		//By default it's pink
		static const uint32_t pixel = 0xffc99aff;
		D3D11_SUBRESOURCE_DATA initData = { &pixel, sizeof(uint32_t), 0 };


		D3D11_TEXTURE2D_DESC desc { };

		desc.Width = 1;
		desc.Height = 1;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		ID3D11Texture2D* texture = nullptr;
		S2DE_CHECK(Core::Engine::GetRenderer()->GetDevice()->CreateTexture2D(&desc, &initData, &texture), "Can't create empty texture");

		D3D11_SHADER_RESOURCE_VIEW_DESC sdesc{};

		sdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		sdesc.Texture2D.MipLevels = 1;

		S2DE_CHECK(Core::Engine::GetRenderer()->GetDevice()->CreateShaderResourceView(texture, &sdesc, &m_resourceview), "Can't create resource (empty texture)");
		UpdateTextureDesc();
		return true;
	}

	void Texture::Bind(std::uint32_t NumViews)
	{
		Core::Engine::GetRenderer()->GetContext()->PSSetSamplers(0, 1, &m_texture_sampler_state);
		Core::Engine::GetRenderer()->GetContext()->PSSetShaderResources(0, NumViews, &m_resourceview);
	}
}