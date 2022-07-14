#include "EditorGrid.h"

namespace S2DE::Editor
{
	EditorGrid::EditorGrid() :
		m_shader(nullptr),
		m_vertexBuffer(new Render::VertexBuffer<Render::Vertex>),
		m_indexBuffer(new Render::IndexBuffer<std::uint32_t>)
	{
		CreateVertexBuffer();
		CreateIndexBuffer();
		SetDefaultShader();
	}

	EditorGrid::~EditorGrid()
	{
		Core::Delete(m_shader);
		Core::Delete(m_indexBuffer);
		Core::Delete(m_vertexBuffer);
	}

	void EditorGrid::OnRender()
	{
		//Bind and update variables in const buffer
		m_shader->UpdateMainConstBuffer(UpdateTransformation());

		//Bind shader and texture 
		m_shader->Bind();

		//Bind buffers
		m_vertexBuffer->Bind();
		m_indexBuffer->Bind();

		//Draw poly 
		Core::Engine::GetRenderer()->GetContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		Core::Engine::GetRenderer()->GetContext()->DrawIndexed(m_indexBuffer->GetArray().size(), 0, 0);

		//Unbind 
		m_shader->Unbind();
		m_vertexBuffer->Unbind();
		m_indexBuffer->Unbind();
	}

	void EditorGrid::CreateVertexBuffer()
	{
		for (std::int32_t z = 0; z < GRID_CELLS; z++)
		{
			for (std::int32_t x = 0; x < GRID_CELLS; x++)
			{
				Render::Vertex vertex = Render::Vertex();
				vertex.position = DirectX::SimpleMath::Vector3(-x, 0.0f, -z);
				vertex.color = DirectX::SimpleMath::Vector4(1, 1, 1, 1);
				m_vertexBuffer->GetArray().push_back(vertex);
			}
		}
	

		S2DE_ASSERT(m_vertexBuffer->Create());
		m_vertexBuffer->Update();
	}

	void EditorGrid::CreateIndexBuffer()
	{
		for (std::int32_t z = 0; z < GRID_CELLS - 1; z++)
		{
			for (std::int32_t x = 0; x < GRID_CELLS - 1; x++)
			{
				m_indexBuffer->GetArray().push_back(z * GRID_CELLS + x);
				m_indexBuffer->GetArray().push_back(z * GRID_CELLS + (x + 1));
				m_indexBuffer->GetArray().push_back((z + 1) * GRID_CELLS + x);
				m_indexBuffer->GetArray().push_back((z + 1) * GRID_CELLS + x);
				m_indexBuffer->GetArray().push_back(z * GRID_CELLS + (x + 1));
				m_indexBuffer->GetArray().push_back((z + 1) * GRID_CELLS + (x + 1));
			}
		}
		S2DE_ASSERT(m_indexBuffer->Create());
		m_indexBuffer->Update();
	}

	void EditorGrid::SetDefaultShader()
	{
		m_shader = new Render::Shader(*Core::Engine::GetResourceManager().Get<Render::Shader>("Line"));
	}
}