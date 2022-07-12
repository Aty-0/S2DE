#include "NoTextureTestObject.h"
#include "GameObjects/Camera.h"
#include "Math/Random.h"
#include "Base/InputManager.h"
#include "Base/GameWindow.h"
#include "Scene/SceneManager.h"

namespace S2DE::GameObjects
{
	NoTextureTestObject::NoTextureTestObject() :
		m_ibuffer(nullptr),
		m_vbuffer(nullptr),
		m_texture(nullptr),
		m_shader(nullptr)
	{
		if (!Core::Engine::GetResourceManager().IsExists<Render::Shader>("Sprite"))
			S2DE_ASSERT(Core::Engine::GetResourceManager().Load<Render::Shader>("Sprite"));

		m_shader = new Render::Shader(*Core::Engine::GetResourceManager().Get<Render::Shader>("Sprite"));
		S2DE_ASSERT(m_shader != nullptr);

		m_texture = new Render::Texture();
		m_texture->CreateEmptyTexture(Math::Color<std::uint32_t>(Math::Random::RandomRange(255), Math::Random::RandomRange(255), Math::Random::RandomRange(255), 100));

		S2DE_ASSERT(m_texture != nullptr);

		m_vbuffer = new Render::VertexBuffer<Render::Vertex>();
		m_vbuffer->GetArray() =
		{
			{ DirectX::SimpleMath::Vector3(-1.0f,   -1.0f,   0.0f),  DirectX::SimpleMath::Vector4(1, 1, 1, 0.5),  DirectX::SimpleMath::Vector2(0.0f, 1.0f) }, 
			{ DirectX::SimpleMath::Vector3(-1.0f,   1.0f,   0.0f),   DirectX::SimpleMath::Vector4(1, 1, 1, 0.5),  DirectX::SimpleMath::Vector2(0.0f, 0.0f) }, 
			{ DirectX::SimpleMath::Vector3(1.0f,  1.0f,   0.0f),	 DirectX::SimpleMath::Vector4(1, 1, 1, 0.5),  DirectX::SimpleMath::Vector2(1.0f, 0.0f) }, 
			{ DirectX::SimpleMath::Vector3(1.0f,  -1.0f,   0.0f),    DirectX::SimpleMath::Vector4(1, 1, 1, 0.5),  DirectX::SimpleMath::Vector2(1.0f, 1.0f) }, 
		};

		S2DE_ASSERT(m_vbuffer->Create());
		m_vbuffer->Update();
		
		m_ibuffer = new Render::IndexBuffer<std::int32_t>();
		m_ibuffer->GetArray() =
		{
				0, 1, 2,
				0, 2, 3,
		};

		S2DE_ASSERT(m_ibuffer->Create());
		m_ibuffer->Update();

		Alpha = true;
	}

	NoTextureTestObject::~NoTextureTestObject()
	{
		//m_shader->Cleanup();
		Core::Delete(m_shader);
		//m_texture->Cleanup();
		Core::Delete(m_texture);
		Core::Delete(m_vbuffer);
		Core::Delete(m_ibuffer);
	}

	void NoTextureTestObject::OnUpdate(float DeltaTime)
	{

	}

	void NoTextureTestObject::OnRender()
	{		
		m_vbuffer->Bind();
		m_ibuffer->Bind();

		m_shader->Bind();
		m_shader->UpdateMainConstBuffer(UpdateTransformation());
		m_texture->Bind();

		Core::Engine::GetRenderer()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Core::Engine::GetRenderer()->GetContext()->DrawIndexed(m_ibuffer->GetArray().size(), 0, 0);

		m_shader->Unbind();
	}

	void NoTextureTestObject::UpdateShader()
	{
		//Get shader name
		std::string name = m_shader->GetName();
		//Delete previous shader 
		Core::Delete(m_shader);

		Render::Shader* upd_sh = Core::Engine::GetResourceManager().Get<Render::Shader>(name);

		if (upd_sh == nullptr)
		{
			Logger::Error("%s Can't update shader!", GetName().c_str());
			return;
		}


		//Try to get shader by name from resource manager
		m_shader = new Render::Shader(*upd_sh);

		//Shader can't be nullptr
		S2DE_ASSERT(m_shader != nullptr);
	}
}
