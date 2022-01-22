#pragma once
#include "GameObjects/Drawable.h"
#include "Base/ResourceManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Buffers.h"
#include "Math/IntVector.h"

namespace S2DE::GameObjects
{
	class S2DE_API Sprite : public Drawable
	{
	public:
		Sprite();
		~Sprite();

		//TODO 
		//Colored texture


		//TODO
		//Load custom shader
		//It's must be template function because we can set custom const buffer type
		//By default it's must be SpriteConstBuffer type


		//Basic load texture function for sprite
		//This function get texture from resource manager and setting it to sprite
		//If texture not found in resource manager and if auto_load_texture == true we try to load it
		//If unload_texture == true we unload the corrent texture when sprite destroying 
		virtual bool	LoadTexture(std::string name, bool unload_texture = false, bool auto_load_texture = true) override;

		//Get new version of current shader from resource manager
		virtual void	UpdateShader() override;

		//Get new version of current texture from resource manager
		virtual void	UpdateTexture() override;

		//Set frame position 
		void			SetAtlasFramePosition(std::int32_t x, std::int32_t y);

		//Set frame size 
		void			SetAtlasSize(Math::Vector2 size);

	private:
		Render::Texture*						m_texture;
		bool									m_unload_texture;
		Render::Shader*							m_shader;
		Render::VertexBuffer<Render::Vertex>*	m_vertex_buffer;
		Render::IndexBuffer<std::int32_t>*		m_index_buffer;
		Math::Vector2							m_tile_size;
		Math::IntVector2						m_tile_frame_pos;

	protected:
		virtual void			OnUpdate(float DeltaTime) override { }
		virtual void			OnRender() override;
		virtual Math::XMatrix	UpdateTransformation() override;

		virtual void			CreateVertexBuffer();
		virtual void			CreateIndexBuffer();
		virtual void			SetDefaultShader();
		virtual void			SetDefaultTexture();
		virtual void			CalcScaleFactor();


		Math::Vector3			ScaleFactor;

	public:
		struct S2DE_ALIGN(16) SpriteConstBuffer
		{
			Math::XInt2				sprite_tile_frame;
			Math::XFloat2			sprite_tile_size;
			Math::XFloat2			sprite_texture_res;
		};

	private:
		Render::ConstantBuffer<SpriteConstBuffer>* m_sprite_const_buf;
	};
}