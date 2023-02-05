#include "Mesh.h"
#include "Render/FBX_Importer.h"

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
	}

	Mesh::~Mesh()
	{
		m_indices.clear();
		m_vertices.clear();
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

		// TODO: Material
		if (format == "fbx")
		{
			if (!FBX_Importer::Import(path, m_vertices, m_indices))
				return false;
		}
		else if (format == "obj")
		{
			
		}

		return true;
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