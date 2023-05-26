#pragma once
#include "GameObjects/Components/DrawableComponent.h"

#define GRID_CELLS 128

namespace S2DE::Render
{
	class Shader;

	template<typename T>
	class VertexBuffer;

	template<typename T>
	class IndexBuffer;
}

namespace S2DE::GameObjects::Components::Editor
{
	class S2DE_API EditorGrid : public GameObjects::Components::DrawableComponent
	{
	public:
		EditorGrid();
		virtual ~EditorGrid();

	protected:
		virtual void OnRender(Render::Renderer* renderer)  override;
					 
		virtual void CreateVertexBuffer();
		virtual void CreateIndexBuffer();
		virtual void SetDefaultShader();

	private:
		Render::Shader* m_shader;
		Render::VertexBuffer<Render::Vertex>*	m_vertexBuffer;
		Render::IndexBuffer<std::uint32_t>*		m_indexBuffer;
	};
}

