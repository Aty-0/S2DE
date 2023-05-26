#pragma once
#include "Base/Main/Common.h"

// Disable fbxsdk warnings
#pragma warning(push, 3)
#pragma warning(disable : 26495)
#pragma warning(disable : 26812)
#pragma warning(disable : 26451)
#include <fbxsdk.h>
#pragma warning(pop)

namespace S2DE::Render
{
	class Mesh;
	class S2DE_API FBX_Importer : public Core::Utils::Singleton<FBX_Importer>
	{
	public:
		FBX_Importer();
		~FBX_Importer();

		void		Destroy();
		void		Init();
		void		PrintNodeInfo(FbxNode* node);
		bool		Import(std::string path, Mesh* _mesh);
		void		GetUV(FbxMesh* mesh, std::int32_t vertexIndex, std::int32_t uvChannel, std::int32_t uvLayer, DirectX::SimpleMath::Vector2& uv);
		void		GetNormal(FbxMesh* mesh, std::int32_t vertexIndex, std::int32_t vertexCount, DirectX::SimpleMath::Vector3& normal);
		void		GetMaterialIndices(FbxMesh* mesh, std::vector<std::int32_t>& meshMatIndices);
		[[nodiscard]] inline FbxManager* GetFbxManager();

	private:
		FbxManager*		m_manager;
	};
}
