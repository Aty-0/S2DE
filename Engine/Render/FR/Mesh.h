#pragma once
#include "Base/Main/Common.h"
#include "IO/IO_File.h"
#include "IO/IO_Disposible.h"

namespace S2DE::Render::FR
{
	class S2DE_API Mesh : public IO::IO_File, public IO::IO_Disposible
	{
	public:
		Mesh();
		virtual ~Mesh();

		void					  Cleanup() override;
		bool					  Load(std::string path) override;
										  
		[[nodiscard]] inline std::vector<Vertex>		  GetVertices() const;
		[[nodiscard]] inline std::vector<std::uint32_t> GetIndices()  const;

	private:
		std::vector<Vertex>				  m_vertices;
		std::vector<std::uint32_t>		  m_indices;
	};
}