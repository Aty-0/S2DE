#pragma once
#include "GameObjects/GameObject.h"
#include "Base/ResourceManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Buffers.h"

namespace S2DE
{
	class S2DE_API Sprite : public GameObject
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
		virtual bool	LoadTexture(std::string name, bool unload_texture = false, bool auto_load_texture = true);

		//Get new version of current shader from resource manager
		virtual void	UpdateShader();

		//Get new version of current texture from resource manager
		virtual void	UpdateTexture();


		void			SetAtlasFramePosition(std::int32_t x, std::int32_t y);
		void			SetAtlasSize(Vector2 size);

	private:
		Texture*		m_texture;
		bool			m_unload_texture;
		Shader*			m_shader;
		VertexBuffer*	m_vertex_buffer;
		IndexBuffer*	m_index_buffer;
		Vector2			m_tile_size;
		XInt2			m_tile_frame_pos;

	protected:
		virtual void	OnUpdate(float DeltaTime) override { }
		virtual void	OnRender() override;
		virtual XMatrix UpdateTransformation() override;

		virtual void	CreateVertexBuffer();
		virtual void	CreateIndexBuffer();
		virtual void	SetDefaultShader();
		virtual void	SetDefaultTexture();
		virtual void	CalcScaleFactor();
		Vector3			ScaleFactor;

	public:
		struct S2DE_ALIGN(16) SpriteConstBuffer
		{
			XInt2			sprite_tile_frame;
			XFloat3			sprite_tile_size;
			XFloat2			sprite_texture_res;
		};

	private:
		ConstantBuffer<SpriteConstBuffer>* m_sprite_const_buf;
	};
}