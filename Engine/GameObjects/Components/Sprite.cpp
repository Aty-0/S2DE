#include "Sprite.h"
#include "Scene\SceneManager.h"
#include "GameObjects\Base\GameObject.h"
#include "GameObjects\Components\Camera.h"
#include "GameObjects\Components\AlphaComponent.h"

#include "Base/ResourceManager.h"
#include "Render/Renderer.h"
#include "Render/Buffers.h"
#include "Render/Texture.h"
#include "Render/Shader.h"

namespace S2DE::GameObjects::Components
{
	Sprite::Sprite() :
		m_texture(nullptr),
		m_shader(nullptr),
		m_billboard(false),
		m_twoSided(false),
		m_indexBuffer(new Render::IndexBuffer<std::int32_t>()),
		m_vertexBuffer(new Render::VertexBuffer<Render::Vertex>()),
		m_tileFrame(Math::float4::Zero),
		m_color(Math::Color<float>(1.0f, 1.0f, 1.0f, 1.0f))
	{
		CreateVertexBuffer();
		CreateIndexBuffer();
		SetDefaultTexture();
		SetDefaultShader();
	}

	Sprite::~Sprite()
	{		
		Core::Delete(m_vertexBuffer);
		Core::Delete(m_indexBuffer);
		Core::Delete(m_shader);
		Core::Delete(m_spriteCB);
		Core::Delete(m_texture);
	}

	void Sprite::SetColor(Math::Color<float> color)
	{	
		m_color = color;

		// Update vertex buffer
		CreateVertexBuffer();
	}

	void Sprite::SetAtlasFramePosition(Math::float2 position)
	{
		m_tileFrame.x = position.x;
		m_tileFrame.y = position.y;
	}

	void Sprite::SetAtlasSize(Math::float2 size)
	{
		m_tileFrame.z = size.x;
		m_tileFrame.w = size.y;
	}

	bool Sprite::LoadTexture(std::string name)
	{
		const auto resourceManager = Core::Resources::ResourceManager::GetInstance();

		// If texture not found in resource manager storage we try to load it 
		if (!resourceManager->IsExists<Render::Texture>(name))
		{
			if (!resourceManager->Load<Render::Texture>(name))
				return false;
		}

		// Set texture if texture is exist
		m_texture = new Render::Texture(*resourceManager->Get<Render::Texture>(name));
		Assert(m_texture != nullptr, "Failed to load texture");
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

	void Sprite::SetUIMode(bool mode)
	{
		m_uiMode = mode;
	}

	void Sprite::OnRender(Render::Renderer* renderer)
	{
		// Bind and update variables in const buffer
		const auto transform = GetOwner()->GetTransform();
		transform->SetScaleFactor(CalcScaleFactor());

		m_shader->UpdateMainConstBuffer(renderer, m_uiMode ? transform->UpdateTransformation2D() : transform->UpdateTransformation(), m_uiMode);

		m_spriteCB->Lock(renderer);
		m_spriteCB->GetData()->tileFrame	= Math::float2(m_tileFrame.x, m_tileFrame.y);
		m_spriteCB->GetData()->tileSize		= Math::float2(m_tileFrame.z, m_tileFrame.w);
		m_spriteCB->GetData()->textureRes	= Math::float2((float)m_texture->GetWidth(), (float)m_texture->GetHeight());
		m_spriteCB->GetData()->billboard    = m_billboard;
		m_spriteCB->Unlock(renderer);
		m_spriteCB->Bind(renderer, 2);

		// Bind shader and texture 
		m_shader->Bind(renderer);
		m_texture->Bind(renderer);

		// Bind buffers
		m_vertexBuffer->Bind(renderer);
		m_indexBuffer->Bind(renderer);

		// Draw quad
		if (m_twoSided)
		{
			renderer->SetRasterizerState(Render::Api::RasterizerMode::TwoSided);
			renderer->DrawIndexed(m_indexBuffer->GetArray().size(), 0, 0, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		renderer->SetRasterizerState(Render::Api::RasterizerMode::Default);
		renderer->DrawIndexed(m_indexBuffer->GetArray().size(), 0, 0, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		// Unbind 
		m_shader->Unbind(renderer);
		m_texture->Unbind(renderer);
		m_vertexBuffer->Unbind(renderer);
		m_indexBuffer->Unbind(renderer);
		m_spriteCB->Unbind(renderer);
	}


	void Sprite::CreateVertexBuffer()
	{	 
		m_vertexBuffer->GetArray() =
		{
			{ { -1.0f,   -1.0f,   0.0f },  { m_color.r, m_color.g, m_color.b, m_color.a },  { 0.0f, 1.0f } }, // Bottom left.
			{ { -1.0f,   1.0f,   0.0f },   { m_color.r, m_color.g, m_color.b, m_color.a },  { 0.0f, 0.0f } }, // Top left.
			{ { 1.0f,  1.0f,   0.0f },	   { m_color.r, m_color.g, m_color.b, m_color.a },  { 1.0f, 0.0f } }, // top right.
			{ { 1.0f,  -1.0f,   0.0f },    { m_color.r, m_color.g, m_color.b, m_color.a },  { 1.0f, 1.0f } }, // Bottom right.
		};

		Assert(m_vertexBuffer->Create(), "Failed to create vertex buffer");
		m_vertexBuffer->Update(Render::Renderer::GetInstance());
	}	 
		 
	void Sprite::CreateIndexBuffer()
	{	 
		m_indexBuffer->GetArray() =
		{
				0, 1, 2,
				0, 2, 3,
		};

		Assert(m_indexBuffer->Create(), "Failed to create index buffer");
		m_indexBuffer->Update(Render::Renderer::GetInstance());
	}	 
		 
	void Sprite::UpdateTexture()
	{
		const auto resourceManager = Core::Resources::ResourceManager::GetInstance();

		// Get texture name
		const auto name = m_texture->GetName();

		// Try to get texture by name from resource manager
		auto new_texture = resourceManager->Get<Render::Texture>(name);

		// If texture not found
		if (new_texture == resourceManager->Get<Render::Texture>("DefaultTexture"))
		{
			Core::Utils::Logger::Error("%s Can't update texture!", GetName().c_str());
			return;
		}

		m_texture = new Render::Texture(*new_texture);
		Assert(m_shader != nullptr, "Failed to load new texture");
	}
	 
	void Sprite::UpdateShader()
	{
		const auto resourceManager = Core::Resources::ResourceManager::GetInstance();

		// Get shader name
		const auto name = m_shader->GetName();
		
		// Try to get shader by name from resource manager
		auto new_shader = resourceManager->Get<Render::Shader>(name);

		// If shader not found
		if (new_shader == nullptr)
		{
			Core::Utils::Logger::Error("%s Can't update shader!", GetName().c_str());
			return;
		}

		m_shader = new Render::Shader(*new_shader);
		Assert(m_shader != nullptr, "Failed to load new shader");
	}

	void Sprite::OnCreate()
	{
		GetOwner()->CreateComponent<GameObjects::Components::AlphaComponent>();
	}

	void Sprite::SetDefaultShader()
	{	 
		const auto resourceManager = Core::Resources::ResourceManager::GetInstance();
		m_shader = new Render::Shader(*Core::Resources::ResourceManager::GetInstance()->Get<Render::Shader>("Sprite"));
		m_spriteCB = new Render::ConstantBuffer<Render::CB::CB_Sprite>();
		Assert(m_spriteCB->Create(), "Failed to create sprite constant buffer");
	}	 
		 
	inline Math::float3 Sprite::CalcScaleFactor()
	{
		const auto scaleFactor = m_tileFrame.z == 0.0f && m_tileFrame.w == 0.0f ? m_texture->GetWidth() / m_texture->GetHeight() : m_tileFrame.z / m_tileFrame.w;
		return Math::float3(scaleFactor, scaleFactor, 1.0f);
	}

	void Sprite::SetDefaultTexture()
	{
		m_texture = new Render::Texture(*Core::Resources::ResourceManager::GetInstance()->Get<Render::Texture>("DefaultTexture"));
	}
}