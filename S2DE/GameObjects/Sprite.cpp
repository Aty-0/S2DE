#include "Sprite.h"


namespace S2DE
{
	Sprite::Sprite() :
		ScaleFactor(Vector3(1, 1, 1)),
		m_texture(nullptr),
		m_shader(nullptr),
		m_index_buffer(nullptr),
		m_vertex_buffer(nullptr),
		m_tile_size(Vector2(0, 0)),
		m_tile_frame_x(0),
		m_tile_frame_y(0)
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
		Delete(m_shader);
		Delete(m_texture);

		//TODO
		//unload_texture
	}

	void Sprite::SetAtlasFramePosition(std::int32_t x, std::int32_t y)
	{
		m_tile_frame_x = x;
		m_tile_frame_y = y;
	}

	void Sprite::SetAtlasSize(Vector2 size)
	{
		m_tile_size = size;
		CalcScaleFactor();
	}

	bool Sprite::LoadTexture(std::string name, bool unload_texture, bool auto_load_texture)
	{
		m_unload_texture = unload_texture;

		//If texture not be loaded before 
		//and we get default texture when try to get it from resource manager then
		//we try to load needed texture from gamedata
		if (Engine::GetResourceManager().Get<Texture>(name) == Engine::GetResourceManager().GetDefaultTexture()
			&& auto_load_texture == true)
		{
			if (!Engine::GetResourceManager().Load<Texture>(name))
				return false;
		}

		//Set texture 
		m_texture = new Texture(*Engine::GetResourceManager().Get<Texture>(name));

		//Texture can't be null 
		S2DE_ASSERT(m_texture != nullptr);	

		CalcScaleFactor();
		return true;
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

		m_shader->GetConstBuffer()->GetBufferData<SpriteConstBuffer>()->sprite_tile_frame_x = m_tile_frame_x;
		m_shader->GetConstBuffer()->GetBufferData<SpriteConstBuffer>()->sprite_tile_frame_y = m_tile_frame_y;
		m_shader->GetConstBuffer()->GetBufferData<SpriteConstBuffer>()->sprite_tile_size = XFloat2(m_tile_size.x, m_tile_size.y);
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
			XVector(), XVector(), To_XMVector3(GetScale() * ScaleFactor),
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
		 
	void Sprite::UpdateTexture()
	{
		//Get texture name
		std::string name = m_texture->GetName();

		//Delete previous texture
		Delete(m_texture);

		Texture* upd_tx = Engine::GetResourceManager().Get<Texture>(name);

		if (upd_tx == nullptr)
		{
			Logger::Error("%s Can't update texture!", GetName().c_str());
			return;
		}

		//Try to get texture by name from resource manager
		m_texture = new Texture(*upd_tx);

		//Texture can't be nullptr
		S2DE_ASSERT(m_texture != nullptr);
	}
	 
	void Sprite::UpdateShader()
	{
		//Get shader name
		std::string name = m_shader->GetName();
		//Delete previous shader 
		Delete(m_shader);

		Shader* upd_sh = Engine::GetResourceManager().Get<Shader>(name);

		if (upd_sh == nullptr)
		{
			Logger::Error("%s Can't update shader!", GetName().c_str());
			return;
		}


		//Try to get shader by name from resource manager
		m_shader = new Shader(*upd_sh);

		//Shader can't be nullptr
		S2DE_ASSERT(m_shader != nullptr);

		//TODO 
		//We need to use custom type 
		//if we used custom shader with custom const buffer type

		//Create constant buffer with sprite const buffer type
		m_shader->CreateConstBuffer<SpriteConstBuffer>();
	}

	void Sprite::SetDefaultShader()
	{	 
		m_shader = new Shader(*Engine::GetResourceManager().Get<Shader>("Sprite"));
		m_shader->CreateConstBuffer<SpriteConstBuffer>();
	}	 
		 
	void Sprite::CalcScaleFactor()
	{
		//TODO
		//Better scale factor
		//But for now it's ok
		if(m_tile_size.ZeroCordinate())
			ScaleFactor = Vector3(m_texture->GetWidth() * 0.01f, m_texture->GetHeight() * 0.01f, 1.0f);
		else
			ScaleFactor = Vector3(m_tile_size.x * 0.01f, m_tile_size.y * 0.01f, 1.0f);
	}

	void Sprite::SetDefaultTexture()
	{
		m_texture = new Texture(*Engine::GetResourceManager().GetDefaultTexture());
		CalcScaleFactor();
	}
}