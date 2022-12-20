#include "FBX_Importer.h"
#include "Base/Engine.h"
#include "Scene/SceneManager.h"

namespace S2DE::Render
{
	FbxManager* FBX_Importer::m_manager;

	void FBX_Importer::Init()
	{
		m_manager = FbxManager::Create();
        Core::Utils::Logger::Log("[FBX_Importer] FBX SDK Version: %s", m_manager->GetVersion());

		if (m_manager == nullptr)
			S2DE_FATAL_ERROR("Can't create fbx manager");

		FbxIOSettings* ios = FbxIOSettings::Create(m_manager, IOSROOT);
        ios->SetBoolProp(IMP_FBX_MATERIAL, true);
        ios->SetBoolProp(IMP_FBX_TEXTURE, true);
        ios->SetBoolProp(IMP_FBX_LINK, true);
        ios->SetBoolProp(IMP_FBX_SHAPE, true);
        ios->SetBoolProp(IMP_FBX_GOBO, true);
        ios->SetBoolProp(IMP_FBX_ANIMATION, true);
        ios->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);

		m_manager->SetIOSettings(ios);
	}

    void FBX_Importer::GetNormal(FbxMesh* mesh, std::int32_t vertexIndex, std::int32_t vertexCount, DirectX::SimpleMath::Vector3& normal)
    {
        if (mesh == nullptr)
            return;

        FbxLayerElementNormal* normals = mesh->GetElementNormal();

        if (normals == nullptr)
        {
            Logger::Error("[FBX_Importer] Can't get normal element");
            return;
        }

        switch (normals->GetMappingMode())
        {
        case FbxGeometryElement::eByControlPoint:
            switch (normals->GetReferenceMode())
            {
                case FbxGeometryElement::eDirect:
                {
                    normal.x = static_cast<float>(normals->GetDirectArray().GetAt(vertexIndex)[0]);
                    normal.y = static_cast<float>(normals->GetDirectArray().GetAt(vertexIndex)[1]);
                    normal.z = static_cast<float>(normals->GetDirectArray().GetAt(vertexIndex)[2]);
                    break;
                }
                case FbxGeometryElement::eIndexToDirect:
                {
                    std::int32_t id = normals->GetIndexArray().GetAt(vertexIndex);
                    normal.x = static_cast<float>(normals->GetDirectArray().GetAt(id)[0]);
                    normal.y = static_cast<float>(normals->GetDirectArray().GetAt(id)[1]);
                    normal.z = static_cast<float>(normals->GetDirectArray().GetAt(id)[2]);
                    break;
                }

                default:
                    Core::Utils::Logger::Error("[FBX_Importer] Invalid reference");
                    break;
            }
            break;
        case FbxGeometryElement::eByPolygonVertex:
            switch (normals->GetReferenceMode())
            {
                case FbxGeometryElement::eDirect:
                {
                    normal.x = static_cast<float>(normals->GetDirectArray().GetAt(vertexCount)[0]);
                    normal.y = static_cast<float>(normals->GetDirectArray().GetAt(vertexCount)[1]);
                    normal.z = static_cast<float>(normals->GetDirectArray().GetAt(vertexCount)[2]);
                    break;
                }

                case FbxGeometryElement::eIndexToDirect:
                {
                    std::int32_t id = normals->GetIndexArray().GetAt(vertexCount);
                    normal.x = static_cast<float>(normals->GetDirectArray().GetAt(id)[0]);
                    normal.y = static_cast<float>(normals->GetDirectArray().GetAt(id)[1]);
                    normal.z = static_cast<float>(normals->GetDirectArray().GetAt(id)[2]);
                    break;
                }

                default:
                    Core::Utils::Logger::Error("[FBX_Importer] Invalid reference");
                    break;
            }
            break;
        case FbxGeometryElement::eNone:
        case FbxGeometryElement::eByPolygon:
        case FbxGeometryElement::eByEdge:
        case FbxGeometryElement::eAllSame:
            Core::Utils::Logger::Error("[FBX_Importer] Unhandled mapping mode for normals");
            break;
        }
    }

    void FBX_Importer::GetUV(FbxMesh* mesh, std::int32_t vertexIndex, std::int32_t uvChannel, std::int32_t uvLayer, DirectX::SimpleMath::Vector2& uv)
    {
        if (mesh == nullptr)
            return;

        if (mesh->GetElementUVCount() == 0)        
            return;
        
        if (uvLayer > 1 || mesh->GetElementUVCount() <= uvLayer)
            return;

        FbxGeometryElementUV* geomUVElement = mesh->GetElementUV(uvLayer);

        if (geomUVElement == nullptr)
        {
            Logger::Error("[FBX_Importer] Can't get geometry element uv!");
            return;
        }

        switch (geomUVElement->GetMappingMode())
        {
        case FbxGeometryElement::eByControlPoint:
            switch (geomUVElement->GetReferenceMode())
            {
                case FbxGeometryElement::eDirect:
                {
                    uv.x = static_cast<float>(geomUVElement->GetDirectArray().GetAt(vertexIndex).mData[0]);
                    uv.y = 1 - static_cast<float>(geomUVElement->GetDirectArray().GetAt(vertexIndex).mData[1]);
                }
                break;

                case FbxGeometryElement::eIndexToDirect:
                {
                    std::int32_t index = geomUVElement->GetIndexArray().GetAt(vertexIndex);
                    uv.x = static_cast<float>(geomUVElement->GetDirectArray().GetAt(index).mData[0]);
                    uv.y = 1 - static_cast<float>(geomUVElement->GetDirectArray().GetAt(index).mData[1]);
                }
                break;

                default:
                    Core::Utils::Logger::Error("[FBX_Importer] Invalid reference");
                    break;
            }
            break;

        case FbxGeometryElement::eByPolygonVertex:
            switch (geomUVElement->GetReferenceMode())
            {
                case FbxGeometryElement::eDirect:
                case FbxGeometryElement::eIndexToDirect:
                {
                    uv.x = static_cast<float>(geomUVElement->GetDirectArray().GetAt(uvChannel).mData[0]);
                    uv.y = 1 - static_cast<float>(geomUVElement->GetDirectArray().GetAt(uvChannel).mData[1]);
                }
                break;

                default:
                    Core::Utils::Logger::Error("[FBX_Importer] Invalid reference");
                    break;
            }
            break;

        case FbxGeometryElement::eNone:
        case FbxGeometryElement::eByPolygon:
        case FbxGeometryElement::eByEdge:
        case FbxGeometryElement::eAllSame:
            Core::Utils::Logger::Error("[FBX_Importer] Unhandled mapping mode for texture coordinate");
            break;
        }
    }

    void FBX_Importer::PrintNodeInfo(FbxNode* node)
    {
        const char* nodeName = node->GetName();
        FbxDouble3 translation = node->LclTranslation.Get();
        FbxDouble3 rotation = node->LclRotation.Get();
        FbxDouble3 scaling = node->LclScaling.Get();					
        Core::Utils::Logger::Log("[FBX_Importer] [Node] Name: %s Translation: (%f, %f, %f) Rotation: (%f, %f, %f) Scale: (%f, %f, %f)",
        	nodeName,
        	translation[0], translation[1], translation[2],
        	rotation[0], rotation[1], rotation[2],
        	scaling[0], scaling[1], scaling[2]);
    }

    void FBX_Importer::Destroy()
    {
        m_manager->Destroy();
    }

	bool FBX_Importer::Import(std::string path, std::vector<Vertex>& meshVertices, std::vector<std::uint32_t>& meshIndices)
	{
        FbxImporter* importer = FbxImporter::Create(m_manager, "");

	    Core::Utils::Logger::Log("[FBX_Importer] Import %s", path.c_str());

        // Initialize Fbx importer
		if (!importer->Initialize(path.c_str(), -1, m_manager->GetIOSettings()))
		{
			Core::Utils::Logger::Error("[FBX_Importer] Can't import this model %s", path.c_str());
			return false;
		}

        // Check format valid
        if (!importer->IsFBX())
        {
			Core::Utils::Logger::Error("[FBX_Importer] Format is not valid");
            return false;
        }

        // Create import scene 
        FbxScene* scene = FbxScene::Create(m_manager, "ImportScene");		

        if (scene == nullptr)
        {
			Core::Utils::Logger::Error("[FBX_Importer] Scene creation error");
            return false;
        }

        // Import model 
        if (!importer->Import(scene))
        {
			Core::Utils::Logger::Error("[FBX_Importer] Can't import scene!");
            return false;
        }

        if (scene->GetGlobalSettings().GetSystemUnit() != FbxSystemUnit::m)
            FbxSystemUnit::m.ConvertScene(scene);

        // Create geometry conventer
        FbxGeometryConverter geometryConverter(m_manager);

        // Triangulate mesh
        geometryConverter.Triangulate(scene, true, false);
        
        importer->Destroy();
        
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

                if (attributeType == FbxNodeAttribute::EType::eLight)
                {
	                
                }
                else if (attributeType == FbxNodeAttribute::EType::eMesh)
                {
                    FbxMesh* mesh = node->GetMesh();
                    std::uint32_t vertexCount = 0;
                    std::uint32_t indexCount = 0;
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
                            // Get vertex index
                            std::int32_t index = mesh->GetPolygonVertex(poly, polyVert);

                            // FIXME: This cycle add more details for model but still is not fully loads
                            for (std::int32_t j = 0; j < 3; j++)
                                meshIndices.push_back(indexCount++);
    

                            // Create new vertex
                            Vertex vertex = Vertex();
                            // Get vertex position
                            FbxVector4 currentVecPos = vertices[index];
                            vertex.position = DirectX::SimpleMath::Vector3(static_cast<float>(currentVecPos.mData[0]),
                                static_cast<float>(currentVecPos.mData[1]), static_cast<float>(currentVecPos.mData[2]));
                            // Get uv position
                            FBX_Importer::GetUV(mesh, index, mesh->GetTextureUVIndex(poly, polyVert), 0, vertex.uv);
                            // Get normal position
                            FBX_Importer::GetNormal(mesh, index, vertexCount, vertex.normal);

                            // TODO: We need to rework this
                            FbxSurfaceMaterial* surfaceMaterial = node->GetMaterial(i);
                            bool isHasDiffColor = false;
                            if (surfaceMaterial != nullptr)
                            {
                                if (surfaceMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
                                {
                                    FbxDouble3 diffuse = ((FbxSurfacePhong*)surfaceMaterial)->Diffuse;
                                    vertex.color = DirectX::SimpleMath::Vector4(diffuse[0], diffuse[1], diffuse[2], 1);
                                    isHasDiffColor = true;
                                }
                            }

                            if (isHasDiffColor == false)
                            {
                                vertex.color = DirectX::SimpleMath::Vector4(1, 1, 1, 1);
                            }

                            // Push created vertex to list 
                            meshVertices.push_back(vertex);
                            vertexCount++;
                        }
                    }
                }

            }
        }
        scene->Destroy();
		return true;
	}
}