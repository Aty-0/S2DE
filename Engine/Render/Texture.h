#pragma once
#include "Base/Main/Common.h"
#include "Base/Resource.h"
#include "Render/Renderer.h"

namespace S2DE::Render
{
	class S2DE_API Texture : public Core::Resources::Resource
	{
	public:
		Texture();
		~Texture();
		
		bool							 Load(std::string name) final;

		bool							 CreateFontTexture(std::int32_t w, std::int32_t h, std::uint8_t* data);

		// Create cubemap texture from path 
		bool							 CreateCubeMapTexture(std::string path);
		// Create 1x1 blank texture 
		bool							 CreateEmptyTexture(Math::Color<std::uint32_t> color = Math::Color<std::uint32_t>(208, 0, 255, 255));
		// Create default sampler state 
		bool							 CreateDefaultSamplerState();
		bool							 SetSamplerState(D3D11_SAMPLER_DESC const& samplerDesc);
		// Bind texture 
		void							 Bind(std::uint32_t startSlot = 0, std::uint32_t numViews = 1);
		void							 Unbind(std::uint32_t startSlot = 0, std::uint32_t numViews = 1);
		
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

	// FIX ME: maybe we should to put index in Texture class 
	typedef struct texture_indexed
	{
		class Texture* texture;
		std::uint32_t	index;
	} texture_indexed_t;

}
