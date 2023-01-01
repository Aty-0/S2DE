#pragma once
#include "GameObjects/Components/DrawableComponent.h"
#include "Base/ResourceManager.h"
#include "Render/Renderer.h"
#include "Render/Buffers.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Render/Mesh.h"

namespace S2DE::GameObjects::Components
{
	class S2DE_API StaticMesh : public DrawableComponent
	{
	public:
		StaticMesh();
		~StaticMesh();

		bool					LoadMesh(std::string name) override;
		bool					LoadTexture(std::string name) override;
		bool					LoadTextureA(std::string name, bool unload_texture = false, bool auto_load_texture = true) override;

		//Get new version of current shader from resource manager
		void					UpdateShader() override;

		//Get new version of current texture from resource manager
		void					UpdateTexture() override;

		void					SetColor(Math::Color<float> color);
		void					UseIndices(bool use);
	protected:
		void					OnUpdate(float deltaTime) override { }
		void					OnRender() override;
		void					CreateVertexBuffer()	override;
		void					CreateIndexBuffer()	override;
		void					SetDefaultShader()	override;
		void					SetDefaultTexture()	override;

	private:
		Render::Mesh*	 m_mesh;
		Render::Texture* m_texture;
		Render::Texture* m_textureCube;
		bool			 m_unload_texture;
		Render::Shader* m_shader;
		Render::VertexBuffer<Render::Vertex>* m_vertexBuffer;
		Render::IndexBuffer<std::uint32_t>* m_indexBuffer;
		Math::Color<float> m_color;
		bool				m_useIndices;
	};
}

