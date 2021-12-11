#include "Shader.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"

#include "Scene/SceneManager.h"
#include "GameObjects/Camera.h"

#include <fstream>
#include <D3DX11async.h>

namespace S2DE::Render
{
	Shader::Shader() 
	{
		m_type = "Shader";
		m_ex = { ".hlsl", ".shader", ".fx" };
	}

	Shader::~Shader()
	{

	}

	void Shader::Cleanup()
	{
		Core::Release(m_vertexShader);
		Core::Release(m_pixelShader);
		Core::Release(m_layout);
	}

	void Shader::Unbind()
	{
		Core::Engine::GetRenderer()->GetContext()->IASetInputLayout(0);
		Core::Engine::GetRenderer()->GetContext()->VSSetShader(0, nullptr, 0);
		Core::Engine::GetRenderer()->GetContext()->PSSetShader(0, nullptr, 0);
	}


	void Shader::Bind()
	{
		Core::Engine::GetRenderer()->GetContext()->IASetInputLayout(m_layout);
		Core::Engine::GetRenderer()->GetContext()->VSSetShader(m_vertexShader, nullptr, 0);
		Core::Engine::GetRenderer()->GetContext()->PSSetShader(m_pixelShader, nullptr, 0);
	}

	bool Shader::Compile()
	{
		ID3D10Blob* code_buffer;
		ID3D10Blob* err_buffer;

		Logger::Log("[Shader] [%s] Compile vertex shader...", m_name.c_str());
		if (FAILED(D3DX11CompileFromFileA(m_path_vs.c_str(), nullptr, nullptr, "main", "vs_5_0",
			D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &code_buffer, &err_buffer, NULL)))
		{
			std::string details;

			if (err_buffer != nullptr)
			{
				details = (char*)err_buffer->GetBufferPointer();
				Core::Release(err_buffer);
			}
			else
				details = "No description available!";
			

			Logger::Error("[Shader] Failed\nError details:\n%s", details.c_str());
			return false;
		}
		
		if (FAILED(Core::Engine::GetRenderer()->GetDevice()->CreateVertexShader(code_buffer->GetBufferPointer(),
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
				0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		Logger::Log("[Shader] [%s] Try to create input layout...", m_name.c_str());
		if (FAILED(Core::Engine::GetRenderer()->GetDevice()->CreateInputLayout(elements, sizeof(elements) / sizeof(elements[0]), code_buffer->GetBufferPointer(),
			code_buffer->GetBufferSize(), &m_layout)))
		{		
			Logger::Error("[Shader] Failed to create input layout");
			return false;
		}
	

		Core::Release(code_buffer);

		Logger::Log("[Shader] [%s] Compile pixel shader...", m_name.c_str());
		if (FAILED(D3DX11CompileFromFileA(m_path_ps.c_str(), nullptr, nullptr, "main", "ps_5_0",
			D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &code_buffer, &err_buffer, NULL)))
		{
			std::string details;

			if (err_buffer != nullptr)
			{
				details = (char*)err_buffer->GetBufferPointer();
				Core::Release(err_buffer);
			}
			else
				details = "No description available!";


			Logger::Error("[Shader] Failed\nError details:\n%s", details.c_str());
			return false;
		}
		
		if (FAILED(Core::Engine::GetRenderer()->GetDevice()->CreatePixelShader(code_buffer->GetBufferPointer(),
			code_buffer->GetBufferSize(), nullptr, &m_pixelShader)))
		{
			Logger::Error("[Shader] Can't create pixel shader");
			return false;
		}

		Core::Release(code_buffer);

		//Create basic constant buffer
		m_const_buffer = new ConstantBuffer<ShaderMainConstantBuffer>();
		S2DE_ASSERT(m_const_buffer->Create());

		Logger::Log("[Shader] [%s] Ready!", m_name.c_str());
		return true;
	}

	bool Shader::SetPaths(std::string vs, std::string ps)
	{
		if((Core::Other::isStringEmpty(m_path_vs = vs) || Core::Other::isStringEmpty(m_path_ps = ps)))
			return false;

		return Compile();
	}

	void Shader::UpdateMainConstBuffer(Math::XMatrix world)
	{
		m_const_buffer->Lock();
		m_const_buffer->GetBufferData()->Delta = Core::Engine::GetGameTime().GetDeltaTime();
		m_const_buffer->GetBufferData()->Time = Core::Engine::GetGameTime().GetTime();
		m_const_buffer->GetBufferData()->Resoultion = Math::XFloat2((float)Core::Engine::GetGameWindow()->GetWidth(),
			(float)Core::Engine::GetGameWindow()->GetHeight());

		m_const_buffer->GetBufferData()->world = world;

		GameObjects::Camera* cam = Scene::GetObjectByName<GameObjects::Camera>(S2DE_MAIN_CAMERA_NAME);

		if (cam != nullptr)
		{
			m_const_buffer->GetBufferData()->projection = cam->GetProjectionMatrix();
			m_const_buffer->GetBufferData()->view = cam->GetViewMatrix();
		}

		m_const_buffer->Unlock();
		m_const_buffer->Bind();
	}
}