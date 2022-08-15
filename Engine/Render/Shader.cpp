#include "Shader.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "Render/Renderer.h"
#include "Scene/SceneManager.h"
#include "GameObjects/Camera.h"

#include <d3dcompiler.h>
#include <fstream>
#include <sstream>

namespace S2DE::Render
{
	Shader::Shader()
	{
		m_type = "Shader";
		m_ex = { ".hlsl", ".shader", ".fx" };

		m_flags |= D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(_DEBUG) && defined(S2DE_DEBUG_RENDER_MODE)
		m_flags |= D3DCOMPILE_DEBUG;
		m_flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
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

	void Shader::ShowErrorDetails(ID3D10Blob* error_blob)
	{
		std::string details = std::string();

		if (error_blob != nullptr)
		{
			details = (char*)error_blob->GetBufferPointer();
			Core::Release(error_blob);
		}
		else
		{
			details = "No error description available!";
		}

		Logger::Error("[Shader] Compilation failed!\nDetails:\n%s", details.c_str());
	}

	bool Shader::Compile(bool compileVs, bool compilePs)
	{
		if (!compileVs && !compilePs)
			return false; 
		
		ID3D10Blob* code_buffer = nullptr;
		ID3D10Blob* err_buffer = nullptr;

		if (compileVs)
		{
			Logger::Log("[Shader] [%s] Compile vertex shader...", m_name.c_str());
			if (FAILED(D3DCompile(m_fileDataVs.c_str(), m_fileDataVs.size(), m_path_vs.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"main", "vs_5_0", m_flags, 0, &code_buffer, &err_buffer)))
			{
				ShowErrorDetails(err_buffer);
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
					0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

					{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
					0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

					{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT,
					0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

					{ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};

			Logger::Log("[Shader] [%s] Try to create input layout...", m_name.c_str());
			if (FAILED(Core::Engine::GetRenderer()->GetDevice()->CreateInputLayout(elements, sizeof(elements) / sizeof(elements[0]), code_buffer->GetBufferPointer(),
				code_buffer->GetBufferSize(), &m_layout)))
			{
				Logger::Error("[Shader] Failed to create input layout");
				return false;
			}


			Core::Release(code_buffer);
		}

		if (compilePs)
		{
			Logger::Log("[Shader] [%s] Compile pixel shader...", m_name.c_str());
			
			if (FAILED(D3DCompile(m_fileDataPs.c_str(), m_fileDataPs.size(), m_path_ps.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"main", "ps_5_0", m_flags, 0, &code_buffer, &err_buffer)))
			{
				ShowErrorDetails(err_buffer);
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
			m_const_buffer = new ConstantBuffer<CB::CB_Main>();
			S2DE_ASSERT(m_const_buffer->Create());
		}

		return true;
	}

	bool Shader::Reload()
	{
		return SetPaths(m_path_vs, m_path_ps);
	}

	bool Shader::SetPaths(std::string vs_path, std::string ps_path)
	{
		if ((Core::Utils::isStringEmpty(m_path_vs = vs_path) || Core::Utils::isStringEmpty(m_path_ps = ps_path)))
			return false;
		
		std::ifstream fileVs = std::ifstream();
		std::ifstream filePs = std::ifstream();
		bool compileVs = false;
		bool compilePs = false;

		fileVs.exceptions(std::ios_base::badbit | std::ios_base::failbit);
		filePs.exceptions(std::ios_base::badbit | std::ios_base::failbit);
		
		try
		{
			fileVs.open(m_path_vs.c_str(), std::ios::in);
			bool modifyOrEmpty = true;

			std::stringstream stream = std::stringstream();
			stream << fileVs.rdbuf();
			fileVs.close();

			if (!Core::Utils::isStringEmpty(m_fileDataVs))
			{
				if (m_fileDataVs.length() == stream.str().length())
				{
					modifyOrEmpty = false;
				}
				else
				{
					m_fileDataVs.clear();
				}
			}

			if (modifyOrEmpty)
			{
				m_fileDataVs = stream.str();				
				compileVs = true;
			}

			modifyOrEmpty = true;

			stream.clear();
			stream = std::stringstream();

			filePs.open(m_path_ps.c_str(), std::ios::in);
			stream << filePs.rdbuf();
			filePs.close();

			if (!Core::Utils::isStringEmpty(m_fileDataPs))
			{
				if (m_fileDataPs.length() == stream.str().length())
				{
					modifyOrEmpty = false;
				}
				else
				{
					m_fileDataPs.clear();
				}
			}

			if (modifyOrEmpty)
			{
				m_fileDataPs = stream.str();
				compilePs = true;
			}	

			stream.clear();

		}
		catch (std::ifstream::failure e)
		{
			Logger::Error("Something wrong with shader files \n%s \n%s \n%s ", vs_path.c_str(), ps_path.c_str(), e.what());

			fileVs.close();
			filePs.close();
		}

		return Compile(compileVs, compilePs);
	}

	void Shader::UpdateMainConstBuffer(DirectX::SimpleMath::Matrix world, bool isUI)
	{
		m_const_buffer->Lock();

		m_const_buffer->GetData()->deltatime = Core::Engine::GetGameTime().GetDeltaTime();
		m_const_buffer->GetData()->time = Core::Engine::GetGameTime().GetTime();
		m_const_buffer->GetData()->resoultion = DirectX::SimpleMath::Vector2((float)Core::Engine::GetGameWindow()->GetWidth(), (float)Core::Engine::GetGameWindow()->GetHeight());
		m_const_buffer->GetData()->world = world;

		GameObjects::Camera* camera = Scene::GetObjectByName<GameObjects::Camera>(S2DE_MAIN_CAMERA_NAME);

		if (camera != nullptr)
		{
			if (isUI)
			{
				m_const_buffer->GetData()->projection = camera->GetOrthoMatrix();
				m_const_buffer->GetData()->view = DirectX::SimpleMath::Matrix::Identity;
			}
			else
			{
				m_const_buffer->GetData()->projection = camera->GetProjectionMatrix();
				m_const_buffer->GetData()->view = camera->GetViewMatrix();
			}
		}

		m_const_buffer->Unlock();
		m_const_buffer->Bind();
		m_const_buffer->Unbind();
	}
}