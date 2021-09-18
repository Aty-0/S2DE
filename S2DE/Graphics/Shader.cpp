#include "Shader.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "D3DX11async.h"

#include "GameObjects/Camera.h"

#include <fstream>

namespace S2DE
{
	Shader::Shader() 
	{
		m_type = "Shader";
		m_ex = { ".hlsl", ".s", ".fx" };
	}

	Shader::~Shader()
	{

	}

	void Shader::Cleanup()
	{
		Release(m_vertexShader);
		Release(m_pixelShader);
		Release(m_layout);
		Delete(m_mainbuffer);
	}

	void Shader::Unbind()
	{
		Engine::GetRenderer()->GetContext()->IASetInputLayout(0);
		Engine::GetRenderer()->GetContext()->VSSetShader(0, nullptr, 0);
		Engine::GetRenderer()->GetContext()->PSSetShader(0, nullptr, 0);
	}


	void Shader::Bind()
	{
		Engine::GetRenderer()->GetContext()->IASetInputLayout(m_layout);
		Engine::GetRenderer()->GetContext()->VSSetShader(m_vertexShader, nullptr, 0);
		Engine::GetRenderer()->GetContext()->PSSetShader(m_pixelShader, nullptr, 0);
	}

	bool Shader::Compile()
	{
		ID3D10Blob* code_buffer;
		ID3D10Blob* err_buffer;

		
		Logger::Log("[Shader] [%s] Compile vertex shader...", m_name.c_str());
		if (FAILED(D3DX11CompileFromFileA(m_path_vs.c_str(), nullptr, nullptr, "main", "vs_5_0",
			D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &code_buffer, &err_buffer, NULL)))
		{
			Logger::Error("[Shader] Failed");
			if (err_buffer != nullptr)
			{
				Logger::Error("Details:\n%s", (char*)err_buffer->GetBufferPointer());
				Release(err_buffer);
			}
			else
			{
				Logger::Error("Details:\nNo details");
			}

			return false;
		}
		
		if (FAILED(Engine::GetRenderer()->GetDevice()->CreateVertexShader(code_buffer->GetBufferPointer(), 
			code_buffer->GetBufferSize(), nullptr, &m_vertexShader)))
		{
			Logger::Error("[Shader] Can't create vertex shader");
			return false;
		}

		D3D11_INPUT_ELEMENT_DESC elements[] =
		{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
				0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

				{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,
				0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

				{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT,
				0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		Logger::Log("[Shader] [%s] Try to create input layout...", m_name.c_str());
		if (FAILED(Engine::GetRenderer()->GetDevice()->CreateInputLayout(elements, sizeof(elements) / sizeof(elements[0]), code_buffer->GetBufferPointer(),
			code_buffer->GetBufferSize(), &m_layout)))
		{
			Logger::Error("[Shader] Failed to create input layout");
			return false;
		}

		Release(code_buffer);

		Logger::Log("[Shader] [%s] Compile pixel shader...", m_name.c_str());
		if (FAILED(D3DX11CompileFromFileA(m_path_ps.c_str(), nullptr, nullptr, "main", "ps_5_0",
			D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &code_buffer, &err_buffer, NULL)))
		{
			Logger::Error("[Shader] Failed");
			if (err_buffer != nullptr)
			{
				Logger::Error("Details:\n%s", (char*)err_buffer->GetBufferPointer());
				Release(err_buffer);
			}
			else
			{
				Logger::Error("Details:\nNo details");
			}

			return false;
		}
		
		if (FAILED(Engine::GetRenderer()->GetDevice()->CreatePixelShader(code_buffer->GetBufferPointer(),
			code_buffer->GetBufferSize(), nullptr, &m_pixelShader)))
		{
			Logger::Error("[Shader] Can't create pixel shader");
			return false;
		}

		Release(code_buffer);

		
		m_mainbuffer = new ConstantBuffer<MainShaderBuffer>();
		S2DE_ASSERT(m_mainbuffer->Create());
		
		Logger::Log("[Shader] [%s] Ready!", m_name.c_str());
		return true;
	}

	bool Shader::SetPaths(std::string vs, std::string ps)
	{
		if((isStringEmpty(m_path_vs = vs) || isStringEmpty(m_path_ps = ps)))
			return false;

		return Compile();
	}

	void Shader::UpdateMainShaderBuffer(XMatrix world)
	{
		m_mainbuffer->Lock();

		m_mainbuffer->GetBufferData()->Delta = Engine::GetGameTime().GetDeltaTime();
		m_mainbuffer->GetBufferData()->Time = Engine::GetGameTime().GetTime();
		m_mainbuffer->GetBufferData()->Resoultion = XFloat2((float)Engine::GetGameWindow()->GetWidth(),
			(float)Engine::GetGameWindow()->GetHeight());

		m_mainbuffer->GetBufferData()->world = world;
		m_mainbuffer->GetBufferData()->projection = Camera::_Camera->GetProjectionMatrix();
		m_mainbuffer->GetBufferData()->view = Camera::_Camera->GetViewMatrix();

		m_mainbuffer->Unlock();

		m_mainbuffer->Bind(0);
	}
}