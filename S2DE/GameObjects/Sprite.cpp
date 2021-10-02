#include "Sprite.h"


namespace S2DE
{
	Sprite::Sprite() :
		m_texture(nullptr),
		m_shader(nullptr),
		m_index_buffer(nullptr),
		m_vertex_buffer(nullptr),
		m_scale_factor(Vector2())
	{
		CreateVertexBuffer();
		CreateIndexBuffer();
		SetDefaultTexture();
		SetDefaultShader();
	}

	Sprite::~Sprite()
	{
		Delete(m_vertex_buffer);
		Delete(m_index_buffer);
	}


	void Sprite::OnUpdate(float DeltaTime)
	{

	}

	void Sprite::OnRender()
	{
		//Bind buffers
		m_vertex_buffer->Bind();
		m_index_buffer->Bind();
		Engine::GetRenderer()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//Bind and update variables in const buffer
		m_shader->ShaderConstBufferBegin();
		m_shader->ShaderConstBufferUpdateBase(UpdateTransformation());

		m_shader->GetConstBuffer()->GetBufferData<SpriteConstBuffer>()->sprite_tile_offset = XFloat2(0, 0);
		m_shader->GetConstBuffer()->GetBufferData<SpriteConstBuffer>()->sprite_tile_size = XFloat2(16, 16);
		m_shader->GetConstBuffer()->GetBufferData<SpriteConstBuffer>()->sprite_texture_res = XFloat2(m_texture->GetWidth(), m_texture->GetHeight());

		m_shader->ShaderConstBufferEnd();

		//Bind shader
		m_shader->Bind();


		//Bind texture
		m_texture->Bind();

		//Draw sprite 
		Engine::GetRenderer()->GetContext()->DrawIndexed(m_index_buffer->GetArray().size(), 0, 0);

		//Unbind buffer
		m_shader->Unbind();
	}

	XMatrix Sprite::UpdateTransformation()
	{
		//Coppyed from transform class  
		GetWorldMatrix() = DirectX::XMMatrixTransformation(
			//Scale
			//Center | Rotation | Scaling

			//Added scale factor
			XVector(), XVector(), To_XMVector3(GetScale() * m_scale_factor),
			//Rotation
			//Center | Quatarnion
			XVector(), ToQuaternion(GetRotation()),
			//Translation
			To_XMVector3(GetPosition()));

		return DirectX::XMMatrixTranspose(GetWorldMatrix());
	}

	void Sprite::CreateVertexBuffer()
	{	 
		m_vertex_buffer = new VertexBuffer();
		m_vertex_buffer->GetArray() =
		{
			{ XFloat3(-1.0f,   -1.0f,   0.0f), XFloat4(255, 255, 255, 255),  XFloat2(0.0f, 1.0f) }, // Bottom left.
			{ XFloat3(-1.0f,   1.0f,   0.0f),  XFloat4(255, 255, 255, 255),  XFloat2(0.0f, 0.0f) }, // Top left.
			{ XFloat3(1.0f,  1.0f,   0.0f), XFloat4(255, 255, 255, 255),  XFloat2(1.0f, 0.0f)	 }, // top right.
			{ XFloat3(1.0f,  -1.0f,   0.0f),  XFloat4(255, 255, 255, 255),  XFloat2(1.0f, 1.0f)	 }, // Bottom right.
		};

		S2DE_ASSERT(m_vertex_buffer->Create());
		m_vertex_buffer->Lock();
		m_vertex_buffer->Unlock();
	}	 
		 
	void Sprite::CreateIndexBuffer()
	{	 
		m_index_buffer = new IndexBuffer();
		m_index_buffer->GetArray() =
		{
				0, 1, 2,
				0, 2, 3,
		};

		S2DE_ASSERT(m_index_buffer->Create());
		m_index_buffer->Lock();
		m_index_buffer->Unlock();
	}	 
		 
	void Sprite::SetDefaultShader()
	{	 
		S2DE_ASSERT(Engine::GetResourceManager().IsExists<Shader>("Sprite"));
		m_shader = new Shader(*Engine::GetResourceManager().Get<Shader>("Sprite"));
		m_shader->CreateConstBuffer<SpriteConstBuffer>();
	}	 
		 
	void Sprite::SetDefaultTexture()
	{
		m_texture = new Texture(*Engine::GetResourceManager().GetDefaultTexture());
		m_scale_factor = Vector3(m_texture->GetWidth() * 0.01f, m_texture->GetHeight() * 0.01f, 1.0f);
	}
}