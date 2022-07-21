#pragma once
#include "GameObjects/Drawable.h"
#include "Base/ResourceManager.h"
#include "Render/Renderer.h"
#include "Render/Buffers.h"

#define GRID_CELLS 128

namespace S2DE::Editor
{
	class S2DE_API EditorGrid : public GameObjects::Drawable
	{
	public:
		EditorGrid();
		~EditorGrid();

	protected:
		virtual void				 OnRender()  override;
		virtual void				 CreateVertexBuffer() override;
		virtual void				 CreateIndexBuffer()  override;
		virtual void				 SetDefaultShader()	 override;

	private:
		Render::Shader* m_shader;
		Render::VertexBuffer<Render::Vertex>*	m_vertexBuffer;
		Render::IndexBuffer<std::uint32_t>*		m_indexBuffer;
	};
}

