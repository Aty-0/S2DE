#pragma once
#include "GameObjects/GameObject.h"
#include "Base/ResourceManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Buffers.h"

namespace S2DE
{
	class S2DE_API SpriteConstBuffer : public ShaderMainBufferType
	{
	public:
		XFloat2 sprite_tile_size;
		XFloat2 sprite_tile_offset;
		XFloat2 sprite_texture_res;
	};

	class S2DE_API Sprite : public GameObject
	{
	public:
		Sprite();
		~Sprite();

	private:
		Texture*		m_texture;
		bool			m_unload_texture;
		Shader*			m_shader;
		VertexBuffer*	m_vertex_buffer;
		IndexBuffer*	m_index_buffer;
	protected:
		virtual void	OnUpdate(float DeltaTime) override;
		virtual void	OnRender() override;
		virtual XMatrix UpdateTransformation() override;
		virtual void	CreateVertexBuffer();
		virtual void	CreateIndexBuffer();
		virtual void	SetDefaultShader();
		virtual void	SetDefaultTexture();

		Vector3			m_scale_factor;
	};
}