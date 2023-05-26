#pragma once
#include "GameObjects/Components/DrawableComponent.h"

namespace S2DE::Render
{
	class Texture;
	class Shader;

	template<typename T>
	class VertexBuffer;

	template<typename T>
	class IndexBuffer;
}

namespace S2DE::GameObjects::Components
{
	class S2DE_API Skybox : public Components::DrawableComponent
	{
	public:
		Skybox();
		~Skybox();

		bool LoadTexture(std::string name) override;

	protected:
		void OnRender(Render::Renderer* renderer) override;
		void CreateVertexBuffer();
		void CreateIndexBuffer();


		[[nodiscard]] inline Math::float4x4	UpdateTransformation();

	private:
		Render::Texture*						m_texture;
		Render::Shader*							m_shader;
		Render::VertexBuffer<Render::Vertex>*	m_vertexBuffer;
		Render::IndexBuffer<std::int32_t>*		m_indexBuffer;
	};
}
