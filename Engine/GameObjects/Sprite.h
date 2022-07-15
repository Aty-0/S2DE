#pragma once
#include "GameObjects/Drawable.h"
#include "Base/ResourceManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Buffers.h"

namespace S2DE::GameObjects
{
	struct S2DE_ALIGN(16) SpriteConstBuffer
	{
		DirectX::XMINT2					sprite_tile_frame;
		DirectX::SimpleMath::Vector2	sprite_tile_size;
		DirectX::SimpleMath::Vector2	sprite_texture_res;
	};

	class S2DE_API Sprite : public Drawable
	{
	public:
		Sprite();
		virtual ~Sprite();

		//Basic load texture function for sprite
		//This function get texture from resource manager and setting it to sprite
		//If texture not found in resource manager and if auto_load_texture == true we try to load it
		//If unload_texture == true we unload the corrent texture when sprite destroying 
		virtual bool			LoadTexture(std::string name);
		virtual bool			LoadTextureA(std::string name, bool unload_texture = false, bool auto_load_texture = true) override;

		//Get new version of current shader from resource manager
		virtual void			UpdateShader() override;

		//Get new version of current texture from resource manager
		virtual void			UpdateTexture() override;

		//Set frame position 
		void					SetAtlasFramePosition(std::int32_t x, std::int32_t y);

		//Set frame size		 
		void					SetAtlasSize(DirectX::SimpleMath::Vector2 size);

		void					SetColor(Math::Color<float> color);

	protected:
		virtual void			OnUpdate(float DeltaTime) override { }
		virtual void			OnRender() override;

		virtual void			CreateVertexBuffer();
		virtual void			CreateIndexBuffer();
		virtual void			SetDefaultShader();
		virtual void			SetDefaultTexture();
		virtual inline DirectX::SimpleMath::Vector3 CalcScaleFactor();
		virtual inline DirectX::SimpleMath::Matrix	UpdateTransformation() override;

	private:
		Render::Texture*							m_texture;
		bool										m_unload_texture;
		Render::Shader*								m_shader;
		Render::VertexBuffer<Render::Vertex>*		m_vertex_buffer;
		Render::IndexBuffer<std::int32_t>*			m_index_buffer;
		DirectX::SimpleMath::Vector2				m_tile_size;
		DirectX::SimpleMath::Rectangle				m_tile_frame_pos;
		Render::ConstantBuffer<SpriteConstBuffer>*	m_sprite_const_buf;
		Math::Color<float>							m_color;
	};
}