#pragma once
#include "Base/Main/Common.h"
#include "Base/Resource.h"
#include "Render/Buffers.h"

namespace S2DE::GameObjects
{
	class GameObject;
}

namespace S2DE::Render
{
	class FBX_Importer;
	class S2DE_API Mesh : public Core::Resources::Resource
	{
	public:
		friend FBX_Importer;

		Mesh();
		virtual ~Mesh();

		bool					  Load(std::string path) final;

		// If we want attaching some model elements to StaticMesh
		void					  SetMeshGameObject(S2DE::GameObjects::GameObject* go);

		[[nodiscard]] inline std::vector<struct texture_indexed> GetTextures()  const;
		[[nodiscard]] inline std::uint32_t 				 GetCountMeshes()  const;
		[[nodiscard]] inline std::vector<Render::VertexBuffer<Render::Vertex>*> GetVertexBuffers()  const;
		[[nodiscard]] inline std::vector<Render::IndexBuffer<std::uint32_t>*>	GetIndexBuffers()  const;
	private:		
		std::vector<Render::VertexBuffer<Render::Vertex>*>	m_vertexBuffers;
		std::vector<Render::IndexBuffer<std::uint32_t>*>	m_indexBuffers;
		std::vector<struct texture_indexed>		m_textures;
		std::uint32_t							m_countMeshes;
		S2DE::GameObjects::GameObject*			m_meshGameObject;
	};
}