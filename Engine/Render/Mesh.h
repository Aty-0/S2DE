#pragma once
#include "Base/Main/Common.h"
#include "Base/Resource.h"
#include "Render/Buffers.h"

namespace S2DE::Render
{
	class S2DE_API Mesh : public Core::Resources::Resource
	{
	public:
		friend class FbxImporter;

		Mesh();
		virtual ~Mesh();

		bool					  Load(std::string path) final;
										  
		[[nodiscard]] inline std::vector<class Texture*> GetTextures()  const;
		[[nodiscard]] inline std::uint32_t 				 GetCountMeshes()  const;
		[[nodiscard]] inline std::vector<Render::VertexBuffer<Render::Vertex>*> GetVertexBuffers()  const;
		[[nodiscard]] inline std::vector<Render::IndexBuffer<std::uint32_t>*>	GetIndexBuffers()  const;
	private:		
		std::vector<Render::VertexBuffer<Render::Vertex>*>	m_vertexBuffers;
		std::vector<Render::IndexBuffer<std::uint32_t>*>	m_indexBuffers;
		std::vector<class Texture*>		m_textures;
		std::uint32_t					m_countMeshes;
	};
}