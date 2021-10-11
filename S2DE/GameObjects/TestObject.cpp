#include "TestObject.h"
#include "GameObjects/Camera.h"
#include "Math/Random.h"
#include "Base/InputManager.h"
#include "Base/GameWindow.h"
#include "Scene/SceneManager.h"

#include <DirectXCollision.h>

#define S2DE_TEST_OBJECT_SPEED 2.0f

namespace S2DE
{
	TestObject::TestObject() : 
		m_r(0.0f),
		m_ibuffer(nullptr),
		m_vbuffer(nullptr),
		m_texture(nullptr),
		m_shader(nullptr)
	{
		if(!Engine::GetResourceManager().IsExists<Shader>("test"))
			S2DE_ASSERT(Engine::GetResourceManager().Load<Shader>("test"));

		if(!Engine::GetResourceManager().IsExists<Texture>("TestObjectTex1"))
			Engine::GetResourceManager().Load<Texture>("TestObjectTex1");

		if(!Engine::GetResourceManager().IsExists<Texture>("TestObjectTex2"))
			Engine::GetResourceManager().Load<Texture>("TestObjectTex2");

		if(!Engine::GetResourceManager().IsExists<Texture>("TestObjectTex3"))
			Engine::GetResourceManager().Load<Texture>("TestObjectTex3");



		m_shader = new Shader(*Engine::GetResourceManager().Get<Shader>("test"));
		S2DE_ASSERT(m_shader != nullptr);

		std::int32_t rnd_tex = RandomByMinMax<std::int32_t>(1, 4);

		Logger::Log("pick texture %d", rnd_tex);

		m_texture = new Texture(*Engine::GetResourceManager().Get<Texture>("TestObjectTex" + std::to_string(rnd_tex)));

		S2DE_ASSERT(m_texture != nullptr);

		m_vbuffer = new VertexBuffer();
		m_vbuffer->GetArray() =
		{
			{ XFloat3(-1.0f,   -1.0f,   0.0f), XFloat4(255, 255, 255, 255),  XFloat2(0.0f, 1.0f) }, // Bottom left.
			{ XFloat3(-1.0f,   1.0f,   0.0f),  XFloat4(255, 255, 255, 255),  XFloat2(0.0f, 0.0f) }, // Top left.
			{ XFloat3(1.0f,  1.0f,   0.0f), XFloat4(255, 255, 255, 255),  XFloat2(1.0f, 0.0f)	 }, // top right.
			{ XFloat3(1.0f,  -1.0f,   0.0f),  XFloat4(255, 255, 255, 255),  XFloat2(1.0f, 1.0f)	 }, // Bottom right.
		};

		S2DE_ASSERT(m_vbuffer->Create());
		m_vbuffer->Lock();
		m_vbuffer->Unlock();

		m_ibuffer = new IndexBuffer();
		m_ibuffer->GetArray() =
		{
				0, 1, 2,
				0, 2, 3,
		};

		S2DE_ASSERT(m_ibuffer->Create());
		m_ibuffer->Lock();
		m_ibuffer->Unlock();

		//m_scale_factor = Vector3(m_texture->GetWidth() * 0.01f, m_texture->GetHeight() * 0.01f, 1.0f);
		m_scale_factor = Vector3(1.0f, 1.0f, 1.0f);

	}

	TestObject::~TestObject()
	{
		m_shader->Cleanup();
		Delete(m_shader);

		m_texture->Cleanup();
		Delete(m_texture);

		Delete(m_vbuffer);
		Delete(m_ibuffer);

		Logger::Log("Destroyed %s", GetName().c_str());
	}

	XMatrix TestObject::UpdateTransformation()
	{
		GetWorldMatrix() = DirectX::XMMatrixTransformation(
			//Scale
			//Center | Rotation | Scaling
			XVector(), XVector(), To_XMVector3(GetScale() * m_scale_factor),
			//Rotation
			//Center | Quatarnion
			XVector(), ToQuaternion(GetRotation()),
			//Translation
			To_XMVector3(GetPosition()));

		return DirectX::XMMatrixTranspose(GetWorldMatrix());
	}

	void TestObject::OnUpdate(float DeltaTime)
	{
		m_r += S2DE_TEST_OBJECT_SPEED * DeltaTime;
		SetRotation(Vector3(0, 0, m_r));
	}

	void TestObject::OnRender()
	{
		m_vbuffer->Bind();
		m_ibuffer->Bind();
		Engine::GetRenderer()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_shader->Bind();
		m_shader->ShaderConstBufferBegin();
		m_shader->ShaderConstBufferUpdateBase(UpdateTransformation());
		m_shader->ShaderConstBufferEnd();
		m_texture->Bind();
		Engine::GetRenderer()->GetContext()->DrawIndexed(m_ibuffer->GetArray().size(), 0, 0);
		m_shader->Unbind();
	}

	bool TestObject::CheckOnIntersection()
	{
		if (Engine::GetInputManager()->IsKeyPressed(KeyCode::KEY_L))
		{

		}
		return false;
	}
}
