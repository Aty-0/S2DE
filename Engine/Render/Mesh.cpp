#include "Mesh.h"
#include "Render/FBX_Importer.h"
#include "Render/Texture.h"

namespace S2DE::Render
{
	Mesh::Mesh()
	{
		m_type = "Model";
		m_extensions =
		{
			".fbx",
			".obj",
		};
		m_countMeshes = 0;
	}

	Mesh::~Mesh()
	{
		m_countMeshes = 0;
		m_indices.clear();
		m_vertices.clear();
		m_textures.clear();
	}

	bool Mesh::Load(std::string name)
	{
		const auto paths = FindPath({ name });
		if (m_notLoaded == true)
		{
			return false;
		}

		const auto path = paths[0];

		std::string format = Core::Utils::GetFileExtension(path);

		if (format == "fbx")
		{
			if (!FBX_Importer::Import(path, m_vertices, m_indices, m_textures, m_countMeshes))
			{
				return false;
			}
		}

		return true;
	}

	inline std::uint32_t Mesh::GetCountMeshes()  const
	{
		return m_countMeshes;
	}

	inline std::vector<Texture*> Mesh::GetTextures()  const
	{		
		return m_textures;
	}

	inline std::vector<Vertex> Mesh::GetVertices() const
	{
		return m_vertices;
	}

	inline std::vector<std::uint32_t> Mesh::GetIndices()  const
	{
		return m_indices;
	}
}