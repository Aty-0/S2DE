#pragma once
#include "Base/Main/Common.h"
#include "Graphics/Renderer.h"
#include "Graphics/Buffers.h"
#include "IO/IO_File.h"
#include "IO/IO_Disposible.h"

namespace S2DE
{
	struct S2DE_API MainShaderBuffer
	{
		float Delta;
		float Time;
		XFloat2 Resoultion;

		//Game object
		XMatrix world;

		//Camera stuff
		XMatrix view;
		XMatrix projection;
	};

	class S2DE_API Shader : public IO_File, public IO_Disposible
	{
	public:
		Shader();
		~Shader();

		bool							 SetPaths(std::string vs, std::string ps);
		bool							 Compile();
		void							 Bind();
		void							 Unbind();
		void							 UpdateMainShaderBuffer(XMatrix world);
		virtual void					 Cleanup() final;
		virtual bool					 Load(std::string path) final { return true; }

		inline ID3D11VertexShader* const GetVertexShader() { return m_vertexShader; }
		inline ID3D11PixelShader*  const GetPixelShader() { return m_pixelShader; }
		inline ID3D11InputLayout*  const GetLayout() { return m_layout; }

	private:

		ID3D11VertexShader*				  m_vertexShader;
		ID3D11PixelShader*				  m_pixelShader;
		ID3D11InputLayout*				  m_layout;
		std::string						  m_path_vs;
		std::string						  m_path_ps;
		ConstantBuffer<MainShaderBuffer>* m_mainbuffer;
	};
}