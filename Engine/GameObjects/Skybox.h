#pragma once

#include "GameObjects/Drawable.h"
#include "Base/ResourceManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Buffers.h"

namespace S2DE::GameObjects
{
	class Skybox : public Drawable
	{
	public:
		Skybox();
		virtual ~Skybox();

	protected:
		virtual void			OnRender() override;
		virtual void			CreateVertexBuffer();
		virtual void			CreateIndexBuffer();
		virtual void			SetDefaultShader();
		virtual void			SetDefaultTexture();
		virtual DirectX::SimpleMath::Matrix	UpdateTransformation() override;
	private:
		Render::Texture* m_texture;
		Render::Shader* m_shader;
		Render::VertexBuffer<Render::Vertex>* m_vertexBuffer;
		Render::IndexBuffer<std::int32_t>* m_indexBuffer;
	};
}
