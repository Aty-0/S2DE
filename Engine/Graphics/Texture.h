#pragma once
#include "Base/Main/Common.h"
#include "IO/IO_File.h"
#include "IO/IO_Disposible.h"

namespace S2DE::Render
{
	class S2DE_API Texture : public IO::IO_File, public IO::IO_Disposible
	{
	public:
		Texture();
		~Texture();
		
		virtual void					 Cleanup() override;
		virtual bool					 Load(std::string path) override;
		virtual bool					 CreateEmptyTexture();


		inline ID3D11ShaderResourceView* GetShaderViewResource() const { return m_resourceview; }
		inline ID3D11Texture2D*			 GetTexture2D() const { return m_texture2d; }
		inline std::uint32_t			 GetWidth() const { return m_texture_desc.Width; }
		inline std::uint32_t			 GetHeight() const { return m_texture_desc.Height; }
		inline std::uint32_t			 GetMipLevels() const { return m_texture_desc.MipLevels; }
		inline DXGI_FORMAT				 GetFormat() const { return m_texture_desc.Format; }
		inline DXGI_SAMPLE_DESC			 GetSampleDesc() const { return m_texture_desc.SampleDesc; }
		inline D3D11_USAGE				 GetUsage() const { return m_texture_desc.Usage; }

		void							 Bind(std::uint32_t NumViews = 1);
	private:
		ID3D11ShaderResourceView*		 m_resourceview;
		ID3D11Resource*					 m_resource;

		ID3D11Texture2D*				 m_texture2d;
		D3D11_TEXTURE2D_DESC			 m_texture_desc;
		ID3D11SamplerState*				 m_texture_sampler_state;

		void							 UpdateTextureDesc();
	};
}
