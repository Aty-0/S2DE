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
		
		virtual void					 Cleanup() override;
		virtual bool					 Load(std::string path) override;
		bool							 CreateCubeMapTexture(std::string path);
		bool							 CreateEmptyTexture(Math::Color<std::uint32_t> color = Math::Color<std::uint32_t>(208, 0, 255, 255));
		bool							 CreateSamplerState();
		void							 Bind(std::uint32_t NumViews = 1);
		void							 Unbind();
		
		inline ID3D11ShaderResourceView* GetResourceView() const { return m_resourceView; }
		inline ID3D11Texture2D*			 GetTexture2D() const { return m_textureHandle; }
		inline std::uint32_t			 GetWidth() const { return m_textureDesc.Width; }
		inline std::uint32_t			 GetHeight() const { return m_textureDesc.Height; }
		inline std::uint32_t			 GetMipLevels() const { return m_textureDesc.MipLevels; }
		inline DXGI_FORMAT				 GetFormat() const { return m_textureDesc.Format; }
		inline DXGI_SAMPLE_DESC			 GetSampleDesc() const { return m_textureDesc.SampleDesc; }
		inline D3D11_USAGE				 GetUsage() const { return m_textureDesc.Usage; }

	private:
		void							 UpdateTextureDesc();

		ID3D11ShaderResourceView*		 m_resourceView;
		ID3D11Resource*					 m_resource;
		ID3D11Texture2D*				 m_textureHandle;
		D3D11_TEXTURE2D_DESC			 m_textureDesc;
		ID3D11SamplerState*				 m_textureSamplerState;
	};
}
