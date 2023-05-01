#pragma once
#include <fbxsdk.h>
#include "Base/Main/Common.h"

namespace S2DE::Render
{
	class Mesh;
	class S2DE_API FBX_Importer
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
		FbxManager* GetFbxManager() { return m_manager; }

	private:
		FbxManager*		m_manager;
	public:
		static FBX_Importer		Importer;
	};
}
