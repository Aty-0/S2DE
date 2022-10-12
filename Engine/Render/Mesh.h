#pragma once
#include "Base/Main/Common.h"
#include "IO/IO_File.h"
#include "IO/IO_Disposible.h"

namespace S2DE::Render
{
	class S2DE_API Mesh : public IO::IO_File, public IO::IO_Disposible
	{
	public:
		Mesh();
		virtual ~Mesh();

		virtual void					  Cleanup() override;
		virtual bool					  Load(std::string path) override;
										  
		inline std::vector<Vertex>		  GetVertices() const { return m_vertices; }
		inline std::vector<std::uint32_t> GetIndices()  const { return m_indices; }

	private:
		std::vector<Vertex>				  m_vertices;
		std::vector<std::uint32_t>		  m_indices;
	};
}