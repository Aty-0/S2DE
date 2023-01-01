#include "Sprite.h"
#include "Scene\SceneManager.h"
#include "GameObjects\Base\GameObject.h"
#include "GameObjects\Components\Camera.h"
#include "GameObjects\Components\AlphaComponent.h"


namespace S2DE::GameObjects::Components
{
	Sprite::Sprite() :
		m_texture(nullptr),
		m_shader(nullptr),
		m_billboard(false),
		m_twoSided(false),
		m_indexBuffer(new Render::IndexBuffer<std::int32_t>()),
		m_vertexBuffer(new Render::VertexBuffer<Render::Vertex>()),
		m_tileFrame(DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f)),
		m_color(Math::Color<float>(1.0f, 1.0f, 1.0f, 1.0f))
	{
		CreateVertexBuffer();
		CreateIndexBuffer();
		SetDefaultTexture();
		SetDefaultShader();
	}

	Sprite::~Sprite()
	{
		//TODO: Need to check count of using
		if (m_unloadTexture == true)
			Core::Engine::GetResourceManager().Erase<Render::Texture>(m_texture->GetName());

		Core::Delete(m_vertexBuffer);
		Core::Delete(m_indexBuffer);
		Core::Delete(m_shader);
		Core::Delete(m_spriteCB);
		Core::Delete(m_texture);
	}

	void Sprite::SetColor(Math::Color<float> color)
	{	
		m_color = color;

		//Update vertex buffer
		CreateVertexBuffer();
	}

	void Sprite::SetAtlasFramePosition(DirectX::SimpleMath::Vector2 position)
	{
		m_tileFrame.x = position.x;
		m_tileFrame.y = position.y;
	}

	void Sprite::SetAtlasSize(DirectX::SimpleMath::Vector2 size)
	{
		m_tileFrame.z = size.x;
		m_tileFrame.w = size.y;
	}

	bool Sprite::LoadTexture(std::string name)
	{
		return LoadTextureA(name);
	}

	bool Sprite::LoadTextureA(std::string name, bool unload_texture, bool auto_load_texture)
	{
		//Set unload state
		m_unloadTexture = unload_texture;

		//If texture not found in resource manager storage we try to load it 
		if (!Core::Engine::GetResourceManager().IsExists<Render::Texture>(name)
			&& auto_load_texture == true)
		{
			if (!Core::Engine::GetResourceManager().Load<Render::Texture>(name))
				return false;
		}
		//Set texture if texture is exist
		m_texture = new Render::Texture(*Core::Engine::GetResourceManager().Get<Render::Texture>(name));
		S2DE_ASSERT(m_texture != nullptr);	
		return true;
	}

	void Sprite::SetBillboard(bool billboard)
	{
		m_billboard = billboard;
	}

	void Sprite::SetTwoSidedMode(bool mode)
	{
		m_twoSided = mode;
	}

	void Sprite::OnRender()
	{
		//Bind and update variables in const buffer
		m_shader->UpdateMainConstBuffer(UpdateTransformation());

		m_spriteCB->Lock();
		m_spriteCB->GetData()->tileFrame	= DirectX::SimpleMath::Vector2(m_tileFrame.x, m_tileFrame.y);
		m_spriteCB->GetData()->tileSize		= DirectX::SimpleMath::Vector2(m_tileFrame.z, m_tileFrame.w);
		m_spriteCB->GetData()->textureRes	= DirectX::SimpleMath::Vector2((float)m_texture->GetWidth(), (float)m_texture->GetHeight());
		m_spriteCB->GetData()->billboard    = m_billboard;
		m_spriteCB->Unlock();
		m_spriteCB->Bind(2);

		//Bind shader and texture 
		m_shader->Bind();
		m_texture->Bind();

		//Bind buffers
		m_vertexBuffer->Bind();
		m_indexBuffer->Bind();

		//Draw poly 
		if (m_twoSided)
		{
			Core::Engine::GetRenderer()->SetRasterizerState("fcc");
			Core::Engine::GetRenderer()->DrawIndexed(m_indexBuffer->GetArray().size(), 0, 0, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		Core::Engine::GetRenderer()->SetRasterizerState();
		Core::Engine::GetRenderer()->DrawIndexed(m_indexBuffer->GetArray().size(), 0, 0, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		//Unbind 
		m_shader->Unbind();
		m_texture->Unbind();
		m_vertexBuffer->Unbind();
		m_indexBuffer->Unbind();
		m_spriteCB->Unbind();
	}

	inline DirectX::SimpleMath::Matrix Sprite::UpdateTransformation()
	{			
		auto transform = GetOwner()->GetTransform();
		auto wMatrix = transform->GetWorldMatrix();
		auto parentPosition = DirectX::SimpleMath::Vector3::Zero;
		auto parentRotation = DirectX::SimpleMath::Vector3::Zero;
		auto parentScale = DirectX::SimpleMath::Vector3::One;

		// TODO: Get global position, rotation, scale 
		//		 This is local p, r, s
		if (transform->GetParent() != nullptr)
		{
			auto transformParent = transform->GetParent()->GetTransform();

			if (transformParent != nullptr)
			{
				parentPosition = transformParent->GetPosition();
				parentRotation = transformParent->GetRotation();
				parentScale = transformParent->GetScale();
			}
		}

		wMatrix = DirectX::XMMatrixTransformation(
			//Scale
			//Center | Rotation | Scaling
			DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero, transform->GetScale() * parentScale * CalcScaleFactor(),
			//Rotation
			//Center | Quatarnion
			DirectX::SimpleMath::Vector3::Zero, TransformHelpers::ToQuaternion(transform->GetRotation() + parentRotation),
			//Translation
			transform->GetPosition() + parentPosition);

		wMatrix.Transpose(wMatrix);

		return wMatrix;
	}

	void Sprite::CreateVertexBuffer()
	{	 
		m_vertexBuffer->GetArray() =
		{
			{ DirectX::SimpleMath::Vector3(-1.0f,   -1.0f,   0.0f),  DirectX::SimpleMath::Vector4(m_color.r, m_color.g, m_color.b, m_color.a),  DirectX::SimpleMath::Vector2(0.0f, 1.0f) }, // Bottom left.
			{ DirectX::SimpleMath::Vector3(-1.0f,   1.0f,   0.0f),   DirectX::SimpleMath::Vector4(m_color.r, m_color.g, m_color.b, m_color.a),  DirectX::SimpleMath::Vector2(0.0f, 0.0f) }, // Top left.
			{ DirectX::SimpleMath::Vector3(1.0f,  1.0f,   0.0f),	 DirectX::SimpleMath::Vector4(m_color.r, m_color.g, m_color.b, m_color.a),  DirectX::SimpleMath::Vector2(1.0f, 0.0f) }, // top right.
			{ DirectX::SimpleMath::Vector3(1.0f,  -1.0f,   0.0f),    DirectX::SimpleMath::Vector4(m_color.r, m_color.g, m_color.b, m_color.a),  DirectX::SimpleMath::Vector2(1.0f, 1.0f) }, // Bottom right.
		};

		S2DE_ASSERT(m_vertexBuffer->Create());
		m_vertexBuffer->Update();
	}	 
		 
	void Sprite::CreateIndexBuffer()
	{	 
		m_indexBuffer->GetArray() =
		{
				0, 1, 2,
				0, 2, 3,
		};

		S2DE_ASSERT(m_indexBuffer->Create());
		m_indexBuffer->Update();
	}	 
		 
	void Sprite::UpdateTexture()
	{
		//Get texture name
		std::string name = m_texture->GetName();
		//Delete previous texture
		Core::Delete(m_texture);

		//Try to get texture by name from resource manager
		auto new_texture = Core::Engine::GetResourceManager().Get<Render::Texture>(name);

		//If texture not found
		if (new_texture == Core::Engine::GetResourceManager().Get<Render::Texture>("DefaultTexture"))
		{
			Logger::Error("%s Can't update texture!", GetName().c_str());
			return;
		}

		m_texture = new Render::Texture(*new_texture);
		S2DE_ASSERT(m_texture != nullptr);
	}
	 
	void Sprite::UpdateShader()
	{
		//Get shader name
		std::string name = m_shader->GetName();
		//Delete previous shader 
		Core::Delete(m_shader);

		//Try to get shader by name from resource manager
		auto new_shader = Core::Engine::GetResourceManager().Get<Render::Shader>(name);

		//If shader not found
		if (new_shader == nullptr)
		{
			Logger::Error("%s Can't update shader!", GetName().c_str());
			return;
		}

		m_shader = new Render::Shader(*new_shader);
		S2DE_ASSERT(m_shader != nullptr);		
	}

	void Sprite::OnCreate()
	{
		GetOwner()->CreateComponent<GameObjects::Components::AlphaComponent>();
	}

	void Sprite::SetDefaultShader()
	{	 
		m_shader = new Render::Shader(*Core::Engine::GetResourceManager().Get<Render::Shader>("Sprite"));
		m_spriteCB = new Render::ConstantBuffer<Render::CB::CB_Sprite>();
		S2DE_ASSERT(m_spriteCB->Create());
	}	 
		 
	inline DirectX::SimpleMath::Vector3 Sprite::CalcScaleFactor()
	{
		return m_tileFrame.z == 0.0f && m_tileFrame.w == 0.0f ? DirectX::SimpleMath::Vector3(m_texture->GetWidth() * 0.01f, m_texture->GetHeight() * 0.01f, 1.0f) :
			DirectX::SimpleMath::Vector3(m_tileFrame.z * 0.01f, m_tileFrame.w * 0.01f, 1.0f);
	}

	void Sprite::SetDefaultTexture()
	{
		m_texture = new Render::Texture(*Core::Engine::GetResourceManager().Get<Render::Texture>("DefaultTexture"));
	}
}