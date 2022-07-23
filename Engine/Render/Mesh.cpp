#include "Mesh.h"
#include "FBX_Importer.h"

namespace S2DE::Render
{
	Mesh::Mesh()
	{
		m_type = "Model";
		m_ex =
		{
			".fbx",
			//TODO: ".obj",
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

		//TODO: Other formats check 
		std::int64_t pos = path.find(".fbx");

		if (pos != std::string::npos)
		{
			FbxScene* scene = nullptr;
			if (!FBX_Importer::Import(path, scene))
				return false;
			
			if (scene == nullptr)
				return false;

			FbxNode* rootNode = scene->GetRootNode();

			if (rootNode)
			{
				for (std::int32_t i = 0; i < rootNode->GetChildCount(); i++)
				{
					FbxNode* node = rootNode->GetChild(i);
					if (node->GetNodeAttribute() == nullptr)
						continue;

				
					FbxNodeAttribute::EType attributeType = node->GetNodeAttribute()->GetAttributeType();

					FBX_Importer::PrintNodeInfo(node);

					if (attributeType == FbxNodeAttribute::EType::eMesh)
					{
						FbxMesh* mesh = node->GetMesh();
						std::uint32_t vertexCount = 0;
						std::uint32_t polyCount = mesh->GetPolygonCount();
						FbxVector4* vertices = mesh->GetControlPoints();
						if (vertices == nullptr)
							continue;

						for (std::uint32_t poly = 0; poly < polyCount; poly++)
						{
							std::int32_t polySize = mesh->GetPolygonSize(poly);
							S2DE_ASSERT(polySize == 3);
							for (std::int32_t polyVert = 0; polyVert < polySize; polyVert++)
							{
								std::int32_t index = mesh->GetPolygonVertex(poly, polyVert);
								m_indices.push_back(vertexCount);
								vertexCount++;

								FbxVector4 vec = vertices[index];
						
								Vertex vertex = Vertex();
								vertex.position = DirectX::SimpleMath::Vector3(vec.mData[0], vec.mData[1], vec.mData[2]);

								FBX_Importer::GetUV(mesh, index, mesh->GetTextureUVIndex(poly, polyVert), 0, vertex.uv);
								FBX_Importer::GetNormal(mesh, index, vertexCount, vertex.normal);

								FbxSurfaceMaterial* surfaceMaterial = node->GetMaterial(i);
								bool isHasDiffColor = false;

								if (surfaceMaterial != nullptr)
								{
									if (surfaceMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
									{
										FbxDouble3 diffuse = ((FbxSurfacePhong*)surfaceMaterial)->Diffuse;
										vertex.color = DirectX::SimpleMath::Vector4(diffuse[0], diffuse[1], diffuse[2], 1);
										//Core::Utils::Logger::Warning("%s %f %f %f", surfaceMaterial->GetName(), vertex.color.x, vertex.color.y, vertex.color.z);
										isHasDiffColor = true;
									}
								}

								if(isHasDiffColor == false)
								{
									//TODO: Random color
									vertex.color = DirectX::SimpleMath::Vector4(1, 1, 1, 1);
								}

								m_vertices.push_back(vertex);
							}
						}
						//Core::Utils::Logger::Warning("polyCount = %d vertexCount = %d", polyCount, vertexCount);
					}
				}
			}
			scene->Destroy();
		}
		else
		{
			//TODO: support other formats...	
			S2DE_NO_IMPL();
			return false;
		}

		return true;
	}

}