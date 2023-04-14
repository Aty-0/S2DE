#pragma once
#include "Base/Main/Common.h"
#include "Base/Resource.h"
#include "Render/ConstantBuffer.h"
#include "Render/CB.h"

namespace S2DE::Render
{
	class S2DE_API Shader : public Core::Resources::Resource
	{
	public:
		Shader();
		~Shader();

		bool								 Reload();
		bool								 Load(std::string name) final;

		// Compile shaders					 
		bool								 Compile(bool compileVs = true, bool compilePs = true);

		// Bind shader 						 
		void								 Bind();

		// Unbind shader						 
		void								 Unbind();

		void								 UpdateMainConstBuffer(DirectX::SimpleMath::Matrix world, bool isUI = false);

		[[nodiscard]] inline ID3D11VertexShader* GetVertexShader()	 const;
		[[nodiscard]] inline ID3D11PixelShader* GetPixelShader()	 const;
		[[nodiscard]] inline ID3D11InputLayout* GetLayout() const;
		[[nodiscard]] inline ConstantBuffer<CB::CB_Main>* GetConstBuffer()	 const;

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

		// TODO: Remove this and make same function for ResourceManager 
		bool								 CheckShadersOnModify(std::string path, std::string& fileData, bool& modify);
	};
}