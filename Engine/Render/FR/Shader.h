#pragma once
#include "Base/Main/Common.h"
#include "IO/IO_File.h"
#include "IO/IO_Disposible.h"

#include "Render/ConstantBuffer.h"
#include "Render/CB.h"

namespace S2DE::Render::FR
{
	class S2DE_API Shader : public IO::IO_File, public IO::IO_Disposible
	{
	public:
		Shader();
		~Shader();

		bool								 Reload();
		//Set shader files path 
		bool								 SetPaths(std::string vs_path, std::string ps_path);
		//Compile shaders					 
		bool								 Compile(bool compileVs = true, bool compilePs = true);
		//Bind shader 						 
		void								 Bind();
		//Unbind shader						 
		void								 Unbind();
		//Release all resource				 
		void								 Cleanup() final;

		void								 UpdateMainConstBuffer(DirectX::SimpleMath::Matrix world, bool isUI = false);

		[[nodiscard]] inline ID3D11VertexShader* GetVertexShader()	 const;
		[[nodiscard]] inline ID3D11PixelShader* GetPixelShader()	 const;
		[[nodiscard]] inline ID3D11InputLayout* GetLayout() const;
		[[nodiscard]] inline ConstantBuffer<CB::CB_Main>* GetConstBuffer()	 const	;

	private:
		std::string							 m_path_vs;
		std::string							 m_path_ps;
		ID3D11VertexShader*					 m_vertexShader;
		ID3D11PixelShader*					 m_pixelShader;
		ID3D11InputLayout*					 m_layout;
		DWORD								 m_flags;
		ConstantBuffer<CB::CB_Main>*		 m_const_buffer;
		std::string 						 m_fileDataVs;
		std::string 						 m_fileDataPs;

		void								 ShowErrorDetails(ID3D10Blob* error_blob);
	};
}