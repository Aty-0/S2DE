#pragma once
#include "GameObjects/Base/Drawable.h"
#include "Base/ResourceManager.h"
#include "Render/Renderer.h"
#include "Render/Buffers.h"

namespace S2DE::GameObjects
{
	class S2DE_API StaticMesh : public Drawable
	{
	public:
		StaticMesh();
		virtual ~StaticMesh();

		virtual bool			LoadMesh(std::string name);
		virtual bool			LoadTexture(std::string name);
		virtual bool			LoadTextureA(std::string name, bool unload_texture = false, bool auto_load_texture = true) override;

		//Get new version of current shader from resource manager
		virtual void			UpdateShader() override;

		//Get new version of current texture from resource manager
		virtual void			UpdateTexture() override;

		void					SetColor(Math::Color<float> color);
		void					UseIndices(bool use);
	protected:
		virtual void			OnUpdate(float DeltaTime) override { }
		virtual void			OnRender() override;
		virtual void			CreateVertexBuffer();
		virtual void			CreateIndexBuffer();
		virtual void			SetDefaultShader();
		virtual void			SetDefaultTexture();

	private:
		Render::Mesh* m_mesh;
		Render::Texture* m_texture;
		bool			 m_unload_texture;
		Render::Shader* m_shader;
		Render::VertexBuffer<Render::Vertex>* m_vertexBuffer;
		Render::IndexBuffer<std::uint32_t>* m_indexBuffer;
		Math::Color<float> m_color;
		bool				m_useIndices;
	};
}

