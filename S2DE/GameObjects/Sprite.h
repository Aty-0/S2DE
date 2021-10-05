#pragma once
#include "GameObjects/GameObject.h"
#include "Base/ResourceManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Buffers.h"

namespace S2DE
{
	struct S2DE_API SpriteConstBuffer : ShaderMainBufferType
	{
		int			 sprite_tile_frame_x;
		int			 sprite_tile_frame_y;
		XFloat2		 sprite_tile_size;
		XFloat2		 sprite_texture_res;
	};

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
		virtual bool	LoadTexture(std::string name, bool unload_texture = true, bool auto_load_texture = true);

		void			SetAtlasFramePosition(std::int32_t x, std::int32_t y);
		void			SetAtlasSize(Vector2 size);

		//Get new version of current shader from resource manager
		virtual void	UpdateShader();

		//Get new version of current texture from resource manager
		virtual void	UpdateTexture();

	private:
		Texture*		m_texture;
		bool			m_unload_texture;
		Shader*			m_shader;
		VertexBuffer*	m_vertex_buffer;
		IndexBuffer*	m_index_buffer;
		Vector2			m_tile_size;
		std::int32_t	m_tile_frame_x;
		std::int32_t	m_tile_frame_y;

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
	};
}