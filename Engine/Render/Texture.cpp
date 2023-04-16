#include "Texture.h"
#include "Base/Engine.h"
#include "Render/Renderer.h"

#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

namespace S2DE::Render
{
	Texture::Texture() :
			m_resourceView(nullptr),
			m_resource(nullptr),
			m_textureHandle(nullptr),
			m_textureSamplerState(nullptr)
	{
		m_type = "Texture";
		m_extensions = 
		{ 
			// DDS loader supports
			".dds", 
			// WIC loader supports 
			".bmp", 
			".tiff", 
			".gif",  
			".png", 
			".tga", 
			".jpg" 
		};
	}

	Texture::~Texture()
	{
		Core::Release(m_resource);
		Core::Release(m_resourceView);
		Core::Release(m_textureHandle);
		Core::Release(m_textureSamplerState);
		
	}

	void Texture::UpdateTextureDesc()
	{
		m_resourceView->GetResource(&m_resource);
		m_resource->QueryInterface<ID3D11Texture2D>(&m_textureHandle);
		m_textureHandle->GetDesc(&m_textureDesc);
	}

	bool Texture::SetSamplerState(D3D11_SAMPLER_DESC const& samplerDesc)
	{
		Verify_HR(Core::Engine::GetRenderer()->GetDevice()->CreateSamplerState(&samplerDesc, &m_textureSamplerState), "Can't create sampler state");
		return true;
	}

	bool Texture::CreateDefaultSamplerState()
	{
		D3D11_SAMPLER_DESC sampler_desc = { };
		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
		sampler_desc.MipLODBias = 0.0f;
		sampler_desc.MaxAnisotropy = 1;
		sampler_desc.BorderColor[0] = 0;
		sampler_desc.BorderColor[1] = 0;
		sampler_desc.BorderColor[2] = 0;
		sampler_desc.BorderColor[3] = 0;

		Verify_HR(Core::Engine::GetRenderer()->GetDevice()->CreateSamplerState(&sampler_desc, &m_textureSamplerState), "Can't create sampler state");
		return true;
	}

	bool Texture::Load(std::string name)
	{
		const auto paths = FindPath({ name });
		if (m_notLoaded == true)
		{
			return false;
		}

		const auto path = paths[0];
		HRESULT hr = S_OK;

		// Get file extension because for dds format we need to use special function
		std::int64_t pos = path.find(".dds");
		if (pos != std::string::npos)
		{
			hr = DirectX::CreateDDSTextureFromFileEx(Core::Engine::GetRenderer()->GetDevice(), Core::Utils::StringToWString(path).c_str(), 
				0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, false, &m_resource, &m_resourceView);

			if (FAILED(hr))
			{
				Logger::Error("Can't create dds texture from file Path:%s Details:%s", path.c_str(), Logger::GetHRCodeDetails(hr).c_str());
				return false;
			}
		}
		else
		{
			hr = DirectX::CreateWICTextureFromFileEx(Core::Engine::GetRenderer()->GetDevice(), 
				Core::Utils::StringToWString(path).c_str(), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
				DirectX::WIC_LOADER_FLAGS::WIC_LOADER_IGNORE_SRGB, &m_resource, &m_resourceView);

			if (FAILED(hr))
			{
				Logger::Error("Can't create texture from file Path:%s Details:%s", path.c_str(), Logger::GetHRCodeDetails(hr).c_str());
				return false;
			}
		}
		
		// Create sampler state for current texture
		CreateDefaultSamplerState();

		// Get and save texture description
		UpdateTextureDesc();
		return true;
	}

	bool Texture::CreateCubeMapTexture(std::string path)
	{
		// If path is empty 
		if (Core::Utils::isStringEmpty(path))
		{
			Logger::Error("Path string is empty, can't load texture!");
			CreateEmptyTexture(Math::Color<std::uint32_t>(255, 0, 0, 255));
			return false;
		}

		HRESULT hr = S_OK;

		// Get file extension because for dds format we need to use special function
		std::int64_t pos = path.find(".dds");
		if (pos != std::string::npos)
		{
			hr = DirectX::CreateDDSTextureFromFileEx(Core::Engine::GetRenderer()->GetDevice(), Core::Utils::StringToWString(path).c_str(),
				0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, D3D11_RESOURCE_MISC_TEXTURECUBE,
				false, &m_resource, &m_resourceView);

			if (FAILED(hr))
			{
				Logger::Error("Can't create dds texture from file Path:%s Details:%s", path.c_str(), Logger::GetHRCodeDetails(hr).c_str());
				return false;
			}
		}
		else
		{
			Logger::Error("Can't create cube map for this extension, supported only dds! Path:%s", path.c_str());
			CreateEmptyTexture(Math::Color<std::uint32_t>(255, 0, 0, 255));
			return false;
		}

		CreateDefaultSamplerState();
		return true;
	}

	bool Texture::CreateFontTexture(std::int32_t w, std::int32_t h, std::uint8_t* data)
	{
		D3D11_SUBRESOURCE_DATA initData = { };

		initData.pSysMem = (void*)data;
		initData.SysMemPitch = w; // * sizeof(std::uint8_t*);
		initData.SysMemSlicePitch = w * h;

		D3D11_TEXTURE2D_DESC texture_desc = { };

		texture_desc.Width = w;
		texture_desc.Height = h;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8_UNORM;//DXGI_FORMAT_R32G32B8A2_UNORM;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_IMMUTABLE;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		ID3D11Texture2D* texture = nullptr;
		Verify_HR(Core::Engine::GetRenderer()->GetDevice()->CreateTexture2D(&texture_desc, &initData, &texture), "Can't create font texture");

		D3D11_SHADER_RESOURCE_VIEW_DESC shader_desc = { };

		shader_desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8_UNORM;
		shader_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shader_desc.Texture2D.MipLevels = 1;

		Verify_HR(Core::Engine::GetRenderer()->GetDevice()->CreateShaderResourceView(texture, &shader_desc, &m_resourceView), "Can't create shader resource for font texture");

		UpdateTextureDesc();
		CreateDefaultSamplerState();
		return true;
	}

	bool Texture::CreateEmptyTexture(Math::Color<std::uint32_t> color)
	{
		const std::uint32_t pixel = color.r | (color.g << 8) | (color.b << 16) | (color.a << 24); 
		D3D11_SUBRESOURCE_DATA initData = { &pixel, sizeof(uint32_t), 0 };
		D3D11_TEXTURE2D_DESC texture_desc = { };

		texture_desc.Width = 1;
		texture_desc.Height = 1;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.Usage	= D3D11_USAGE_IMMUTABLE;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		ID3D11Texture2D* texture = nullptr;
		Verify_HR(Core::Engine::GetRenderer()->GetDevice()->CreateTexture2D(&texture_desc, &initData, &texture), "Can't create empty texture");

		D3D11_SHADER_RESOURCE_VIEW_DESC shader_desc = { };

		shader_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		shader_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shader_desc.Texture2D.MipLevels = 1;

		Verify_HR(Core::Engine::GetRenderer()->GetDevice()->CreateShaderResourceView(texture, &shader_desc, &m_resourceView), "Can't create shader resource for empty texture");

		UpdateTextureDesc();
		CreateDefaultSamplerState();
		return true;
	}

	void Texture::Unbind(std::uint32_t startSlot, std::uint32_t numViews)
	{
		ID3D11ShaderResourceView* nullSRV = { nullptr };

		Core::Engine::GetRenderer()->GetContext()->PSSetShaderResources(startSlot, numViews, &nullSRV);
		//Core::Engine::GetRenderer()->GetContext()->PSSetSamplers(startSlot, numViews, nullptr);
	}

	void Texture::Bind(std::uint32_t startSlot, std::uint32_t numViews)
	{
		if (m_resourceView != nullptr)
		{
			Core::Engine::GetRenderer()->GetContext()->PSSetShaderResources(startSlot, numViews, &m_resourceView);

			if (m_textureSamplerState != nullptr)
			{
				Core::Engine::GetRenderer()->GetContext()->PSSetSamplers(startSlot, numViews, &m_textureSamplerState);
			}
		}

	}

	inline ID3D11ShaderResourceView* Texture::GetResourceView() const
	{
		return m_resourceView;
	}

	inline ID3D11Texture2D* Texture::GetTexture2D() const
	{
		return m_textureHandle;
	}

	inline std::uint32_t Texture::GetWidth() const
	{
		return m_textureDesc.Width;
	}

	inline std::uint32_t Texture::GetHeight() const
	{
		return m_textureDesc.Height;
	}

	inline std::uint32_t Texture::GetMipLevels() const
	{
		return m_textureDesc.MipLevels;
	}

	inline DXGI_FORMAT	Texture::GetFormat() const
	{
		return m_textureDesc.Format;
	}

	inline DXGI_SAMPLE_DESC	Texture::GetSampleDesc() const
	{
		return m_textureDesc.SampleDesc;
	}

	inline D3D11_USAGE	Texture::GetUsage() const
	{
		return m_textureDesc.Usage;
	}
}