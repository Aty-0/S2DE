#pragma once
#include "Base/Main/Common.h"
#include "Graphics/Renderer.h"
#include "Graphics/Buffers.h"
#include "IO/IO_File.h"
#include "IO/IO_Disposible.h"

namespace S2DE
{
	struct S2DE_API ShaderMainBufferType
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

		//Set shader files path 
		bool							SetPaths(std::string vs, std::string ps);
		//Compile shaders
		bool							Compile();
		//Bind shader 
		void							Bind();
		//Unbind shader
		void							Unbind();
		//Release all resource
		virtual void					Cleanup() final;


		inline ID3D11VertexShader*		GetVertexShader() const { return m_vertexShader; }
		inline ID3D11PixelShader*		GetPixelShader()	 const { return m_pixelShader; }
		inline ID3D11InputLayout*		GetLayout()		 const { return m_layout; }
		inline ConstantBuffer*			GetConstBuffer()		 const { return m_const_buffer; }

	
		//TODO
		//Remove
		virtual bool					Load(std::string path) final { return true; }
	private:
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_layout;
		std::string						  m_path_vs;
		std::string						  m_path_ps;
		ConstantBuffer*					  m_const_buffer;

	public:
		void							ShaderConstBufferBegin();
		void							ShaderConstBufferUpdateBase(XMatrix world);
		void							ShaderConstBufferEnd();

		template<typename T = ShaderMainBufferType>
		void							 CreateConstBuffer()
		{
			m_const_buffer = new ConstantBuffer();
			S2DE_ASSERT(m_const_buffer->Create<T>());
		}

	};
}