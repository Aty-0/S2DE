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
		//TODO
		//Need to replace it to random texture in atlas

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

		Logger::Log("! [test object: %s] pick texture: %d", GetName().c_str(), rnd_tex);

		m_texture = new Render::Texture(*Core::Engine::GetResourceManager().Get<Render::Texture>("TestObjectTex" + std::to_string(rnd_tex)));

		S2DE_ASSERT(m_texture != nullptr);

		m_vbuffer = new Render::VertexBuffer();
		m_vbuffer->GetArray() =
		{
			{ Math::XFloat3(-1.0f,   -1.0f,   0.0f), Math::XFloat4(255, 255, 255, 255),  Math::XFloat2(0.0f, 1.0f) }, // Bottom left.
			{ Math::XFloat3(-1.0f,   1.0f,   0.0f),  Math::XFloat4(255, 255, 255, 255),  Math::XFloat2(0.0f, 0.0f) }, // Top left.
			{ Math::XFloat3(1.0f,  1.0f,   0.0f),	 Math::XFloat4(255, 255, 255, 255),  Math::XFloat2(1.0f, 0.0f)	 }, // top right.
			{ Math::XFloat3(1.0f,  -1.0f,   0.0f),   Math::XFloat4(255, 255, 255, 255),  Math::XFloat2(1.0f, 1.0f)	 }, // Bottom right.
		};

		S2DE_ASSERT(m_vbuffer->Create());
		m_vbuffer->Lock();
		m_vbuffer->Unlock();

		m_ibuffer = new Render::IndexBuffer();
		m_ibuffer->GetArray() =
		{
				0, 1, 2,
				0, 2, 3,
		};

		S2DE_ASSERT(m_ibuffer->Create());
		m_ibuffer->Lock();
		m_ibuffer->Unlock();

		//m_scale_factor = Vector3(m_texture->GetWidth() * 0.01f, m_texture->GetHeight() * 0.01f, 1.0f);
		m_scale_factor = Math::Vector3(1.0f, 1.0f, 1.0f);

	}

	TestObject::~TestObject()
	{
		m_shader->Cleanup();
		Core::Delete(m_shader);
		m_texture->Cleanup();
		Core::Delete(m_texture);
		Core::Delete(m_vbuffer);
		Core::Delete(m_ibuffer);

		Logger::Log("! Destroyed %s", GetName().c_str());
	}

	Math::XMatrix TestObject::UpdateTransformation()
	{
		GetWorldMatrix() = DirectX::XMMatrixTransformation(
			//Scale
			//Center | Rotation | Scaling
			Math::XVector(), Math::XVector(), Math::To_XMVector3(GetScale() * m_scale_factor),
			//Rotation
			//Center | Quatarnion
			Math::XVector(), ToQuaternion(GetRotation()),
			//Translation
			Math::To_XMVector3(GetPosition()));

		return DirectX::XMMatrixTranspose(GetWorldMatrix());
	}

	void TestObject::OnUpdate(float DeltaTime)
	{
		m_r += S2DE_TEST_OBJECT_SPEED * DeltaTime;
		SetRotation(Math::Vector3(0, 0, m_r));
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
}
