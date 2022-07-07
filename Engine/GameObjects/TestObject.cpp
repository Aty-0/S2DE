#include "TestObject.h"
#include "GameObjects/Camera.h"
#include "Math/Random.h"
#include "Base/InputManager.h"
#include "Base/GameWindow.h"
#include "Scene/SceneManager.h"

#include <DirectXCollision.h>

#define S2DE_TEST_OBJECT_SPEED 2.0f

namespace S2DE::GameObjects
{
	TestObject::TestObject() : 
		m_r(0.0f),
		m_ibuffer(nullptr),
		m_vbuffer(nullptr),
		m_texture(nullptr),
		m_shader(nullptr)
	{
		if(!Core::Engine::GetResourceManager().IsExists<Render::Shader>("test"))
			S2DE_ASSERT(Core::Engine::GetResourceManager().Load<Render::Shader>("test"));

		if(!Core::Engine::GetResourceManager().IsExists<Render::Texture>("TestObjectTex1"))
			Core::Engine::GetResourceManager().Load<Render::Texture>("TestObjectTex1");

		if(!Core::Engine::GetResourceManager().IsExists<Render::Texture>("TestObjectTex2"))
			Core::Engine::GetResourceManager().Load<Render::Texture>("TestObjectTex2");

		if(!Core::Engine::GetResourceManager().IsExists<Render::Texture>("TestObjectTex3"))
			Core::Engine::GetResourceManager().Load<Render::Texture>("TestObjectTex3");



		m_shader = new Render::Shader(*Core::Engine::GetResourceManager().Get<Render::Shader>("test"));
		S2DE_ASSERT(m_shader != nullptr);

		std::int32_t rnd_tex = Math::Random::RandomRange<std::int32_t>(1, 4);

		Logger::Log("[test object] pick texture: %d", rnd_tex);

		m_texture = new Render::Texture(*Core::Engine::GetResourceManager().Get<Render::Texture>("TestObjectTex" + std::to_string(rnd_tex)));

		S2DE_ASSERT(m_texture != nullptr);

		m_vbuffer = new Render::VertexBuffer<Render::Vertex>();
		m_vbuffer->GetArray() =
		{
			{ DirectX::SimpleMath::Vector3(-1.0f,   -1.0f,   0.0f),  DirectX::SimpleMath::Vector4(255, 255, 255, 255),  DirectX::SimpleMath::Vector2(0.0f, 1.0f) }, // Bottom left.
			{ DirectX::SimpleMath::Vector3(-1.0f,   1.0f,   0.0f),   DirectX::SimpleMath::Vector4(255, 255, 255, 255),  DirectX::SimpleMath::Vector2(0.0f, 0.0f) }, // Top left.
			{ DirectX::SimpleMath::Vector3(1.0f,  1.0f,   0.0f),	 DirectX::SimpleMath::Vector4(255, 255, 255, 255),  DirectX::SimpleMath::Vector2(1.0f, 0.0f)	 }, // top right.
			{ DirectX::SimpleMath::Vector3(1.0f,  -1.0f,   0.0f),    DirectX::SimpleMath::Vector4(255, 255, 255, 255),  DirectX::SimpleMath::Vector2(1.0f, 1.0f)	 }, // Bottom right.
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


		//m_scale_factor = Vector3(m_texture->GetWidth() * 0.01f, m_texture->GetHeight() * 0.01f, 1.0f);
		m_scale_factor = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	}

	TestObject::~TestObject()
	{
		//m_shader->Cleanup();
		Core::Delete(m_shader);
		//m_texture->Cleanup();
		Core::Delete(m_texture);
		Core::Delete(m_vbuffer);
		Core::Delete(m_ibuffer);

		Logger::Log("[test object] Destroyed %s", GetName().c_str());
	}

	DirectX::SimpleMath::Matrix TestObject::UpdateTransformation()
	{
		//m_WorldMatrix = DirectX::SimpleMath::Matrix::CreateWorld(
		//	m_Position,
		//	DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f),
		//	DirectX::SimpleMath::Vector3::Up);
		//
		//m_WorldMatrix *= DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.z, m_Rotation.x, m_Rotation.y);
		//m_WorldMatrix *= DirectX::SimpleMath::Matrix::CreateScale(m_Scale * m_scale_factor);
		//
		//m_WorldMatrix.Transpose(m_WorldMatrix);
		//
		//return m_WorldMatrix;

		m_WorldMatrix = DirectX::XMMatrixTransformation(
			//Scale
			//Center | Rotation | Scaling
			DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero, m_Scale ,
			//Rotation
			//Center | Quatarnion
			DirectX::SimpleMath::Vector3::Zero, ToQuaternion(m_Rotation),
			//Translation
			m_Position);

		m_WorldMatrix.Transpose(m_WorldMatrix);

		return m_WorldMatrix;
	}

	void TestObject::OnUpdate(float DeltaTime)
	{
		//m_r += S2DE_TEST_OBJECT_SPEED * DeltaTime;
		//SetRotation(DirectX::SimpleMath::Vector3(0, 0, m_r));
	}

	void TestObject::OnRender()
	{
		m_vbuffer->Bind();
		m_ibuffer->Bind();
		Core::Engine::GetRenderer()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_shader->Bind();
		m_shader->UpdateMainConstBuffer(UpdateTransformation());
		m_texture->Bind();
		Core::Engine::GetRenderer()->GetContext()->DrawIndexed(m_ibuffer->GetArray().size(), 0, 0);
		m_shader->Unbind();
	}

	bool TestObject::CheckOnIntersection()
	{
		if (Core::Engine::GetInputManager()->IsKeyPressed(Core::Other::KeyCode::KEY_L))
		{

		}
		return false;
	}

	void TestObject::UpdateTexture()
	{
		//Get texture name
		std::string name = m_texture->GetName();

		//Delete previous texture
		Core::Delete(m_texture);

		Render::Texture* upd_tx = Core::Engine::GetResourceManager().Get<Render::Texture>(name);

		if (upd_tx == nullptr)
		{
			Logger::Error("%s Can't update texture!", GetName().c_str());
			return;
		}

		//Try to get texture by name from resource manager
		m_texture = new Render::Texture(*upd_tx);

		//Texture can't be nullptr
		S2DE_ASSERT(m_texture != nullptr);
	}

	void TestObject::UpdateShader()
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
