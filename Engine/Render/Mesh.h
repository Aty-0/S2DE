#pragma once
#include "Base/Main/Common.h"
#include "Base/Resource.h"

namespace S2DE::Render
{
	class S2DE_API Mesh : public Core::Resources::Resource
	{
	public:
		friend class FbxImporter;

		Mesh();
		virtual ~Mesh();

		bool					  Load(std::string path) final;
										  
		[[nodiscard]] inline std::vector<Vertex>		GetVertices() const;
		[[nodiscard]] inline std::vector<std::uint32_t> GetIndices()  const;
		[[nodiscard]] inline std::vector<class Texture*> GetTextures()  const;
		[[nodiscard]] inline std::uint32_t 			GetCountMeshes()  const;

	private:		
		std::vector<Vertex>				m_vertices;
		std::vector<std::uint32_t>		m_indices;
		std::vector<class Texture*>		m_textures;
		std::uint32_t					m_countMeshes;
	};
}