#pragma once
#include "GameObjects/Drawable.h"
#include "Base/ResourceManager.h"
#include "Render/Renderer.h"
#include "Render/Buffers.h"

namespace S2DE::GameObjects
{
	class S2DE_API Skybox : public Drawable
	{
	public:
		Skybox();
		virtual ~Skybox();

		virtual bool			LoadTexture(std::string name) override;
		virtual bool			LoadTextureA(std::string name, bool unload_texture = false, bool auto_load_texture = false) override;

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
