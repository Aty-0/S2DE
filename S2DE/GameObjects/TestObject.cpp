#include "TestObject.h"
#include "GameObjects/Camera.h"


namespace S2DE
{
	TestObject::TestObject()
	{
		S2DE_ASSERT(Engine::GetResourceManager().Get<Shader>("test") != nullptr);
		m_shader = new Shader(*Engine::GetResourceManager().Get<Shader>("test"));



		m_vbuffer = new VertexBuffer();
		m_vbuffer->GetArray() =
		{
				{ XFloat3(-1.0f,   1.0f,   1.0f), XFloat4(255, 255, 255, 255),  XFloat2(0.0f, 0.0f)	},
				{ XFloat3(1.0f,   1.0f,   1.0f),  XFloat4(255, 255, 255, 255),  XFloat2(1.0f, 0.0f)	},
				{ XFloat3(-1.0f,  -1.0f,   1.0f), XFloat4(255, 255, 255, 255),  XFloat2(0.0f, 1.0f)	},
				{ XFloat3(1.0f,  -1.0f,   1.0f),  XFloat4(255, 255, 255, 255),  XFloat2(1.0f, 1.0f)	},
		};

		S2DE_ASSERT(m_vbuffer->Create());
		m_vbuffer->Lock();
		m_vbuffer->Unlock();

		m_ibuffer = new IndexBuffer();
		m_ibuffer->GetArray() =
		{
				0, 1, 2,
				2, 1, 3,
		};

		S2DE_ASSERT(m_ibuffer->Create());
		m_ibuffer->Lock();
		m_ibuffer->Unlock();
	}

	TestObject::~TestObject()
	{
		m_shader->Cleanup();
		Delete(m_vbuffer);
		Delete(m_ibuffer);
	}

	void TestObject::OnRender()
	{
		m_vbuffer->Bind();
		m_ibuffer->Bind();
		Engine::GetRenderer()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_shader->Bind();
		m_shader->UpdateMainShaderBuffer(UpdateTransformation());
		Engine::GetRenderer()->GetContext()->DrawIndexed(m_ibuffer->GetArray().size(), 0, 0);

		m_shader->Unbind();
	}
}