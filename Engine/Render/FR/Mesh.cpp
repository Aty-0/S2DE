#include "Mesh.h"
#include "Render/FBX_Importer.h"

namespace S2DE::Render::FR
{
	Mesh::Mesh()
	{
		m_type = "Model";
		m_ex =
		{
			".fbx",
			".obj",
		};
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::Cleanup()
	{
		m_indices.clear();
		m_vertices.clear();
	}

	bool Mesh::Load(std::string path)
	{
		//If path is empty 
		if (Core::Utils::isStringEmpty(path))
		{
			Core::Utils::Logger::Error("Path string is empty, can't load mesh!");
			return false;
		}

		std::string format = Core::Utils::GetFileExtension(path);

		// TODO: Material
		if (format == "fbx")
		{
			if (!FBX_Importer::Import(path, m_vertices, m_indices))
				return false;
		}
		else if (format == "obj")
		{
			S2DE_NO_IMPL();
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