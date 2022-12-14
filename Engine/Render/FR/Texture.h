#pragma once
#include "Base/Main/Common.h"
#include "IO/IO_File.h"
#include "IO/IO_Disposible.h"
#include "Render/Renderer.h"

namespace S2DE::Render::FR
{
	class S2DE_API Texture : public IO::IO_File, public IO::IO_Disposible
	{
	public:
		Texture();
		~Texture();
		
		bool							 Load(std::string path) final;
		void							 Cleanup() final;
		// Create cubemap texture from path 
		bool							 CreateCubeMapTexture(std::string path);
		// Create 1x1 blank texture 
		bool							 CreateEmptyTexture(Math::Color<std::uint32_t> color = Math::Color<std::uint32_t>(208, 0, 255, 255));
		// Create default sampler state 
		bool							 CreateDefaultSamplerState();
		bool							 SetSamplerState(D3D11_SAMPLER_DESC const& samplerDesc);
		// Bind texture 
		void							 Bind(std::uint32_t numViews = 1);
		void							 Unbind();
		
		[[nodiscard]] inline ID3D11ShaderResourceView* GetResourceView() const;
		[[nodiscard]] inline ID3D11Texture2D* GetTexture2D() const;
		[[nodiscard]] inline std::uint32_t GetWidth() const;
		[[nodiscard]] inline std::uint32_t GetHeight() const;
		[[nodiscard]] inline std::uint32_t GetMipLevels() const;
		[[nodiscard]] inline DXGI_FORMAT GetFormat() const;
		[[nodiscard]] inline DXGI_SAMPLE_DESC GetSampleDesc() const;
		[[nodiscard]] inline D3D11_USAGE GetUsage() const;

	private:
		void							 UpdateTextureDesc();

		ID3D11ShaderResourceView*		 m_resourceView;
		ID3D11Resource*					 m_resource;
		ID3D11Texture2D*				 m_textureHandle;
		D3D11_TEXTURE2D_DESC			 m_textureDesc;
		ID3D11SamplerState*				 m_textureSamplerState;
	};
}
