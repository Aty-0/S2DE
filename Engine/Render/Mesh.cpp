#include "Mesh.h"
#include "Render/Texture.h"
#include "Render/FBX_Importer.h"
#include "GameObjects/Base/GameObject.h"

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

		m_vertexBuffers.clear();
		m_vertexBuffers.shrink_to_fit();

		m_indexBuffers.clear();
		m_indexBuffers.shrink_to_fit();

		m_textures.clear();
		m_textures.shrink_to_fit();
	}

	void Mesh::SetMeshGameObject(S2DE::GameObjects::GameObject* go)
	{
		m_meshGameObject = go;
		Assert(m_meshGameObject != nullptr, "Can't go be null");
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
			if (!FBX_Importer::GetInstance()->Import(path, this))
			{
				return false;
			}
		}

		Assert(m_vertexBuffers.size() == m_countMeshes, "WTF");
		Assert(m_indexBuffers.size() == m_countMeshes, "WTF");

		return true;
	}

	inline std::uint32_t Mesh::GetCountMeshes()  const
	{
		return m_countMeshes;
	}

	inline std::vector<texture_indexed> Mesh::GetTextures()  const
	{		
		return m_textures;
	}

	inline std::vector<Render::VertexBuffer<Render::Vertex>*> Mesh::GetVertexBuffers()  const
	{
		return m_vertexBuffers;
	}

	inline std::vector<Render::IndexBuffer<std::uint32_t>*>	Mesh::GetIndexBuffers()  const
	{
		return m_indexBuffers;
	}
}