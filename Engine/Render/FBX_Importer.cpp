#include "FBX_Importer.h"
#include "Base/Engine.h"

#include "Render/Texture.h"
#include "Render/Mesh.h"

#include "Scene/SceneManager.h"

#include "GameObjects/Base/GameObject.h"
#include "GameObjects/Components/Light/SpotLight.h"
#include "GameObjects/Components/Light/PointLight.h"
#include "GameObjects/Components/Light/DirectionalLight.h"

namespace S2DE::Render
{
    FBX_Importer::FBX_Importer()
        : m_manager()
    {

    }

    FBX_Importer::~FBX_Importer()
    {

    }

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

    void FBX_Importer::GetNormal(FbxMesh* mesh, std::int32_t vertexIndex, std::int32_t vertexCount, Math::float3& normal)
    {
        if (mesh == nullptr)
            return;

        FbxLayerElementNormal* normals = mesh->GetElementNormal();

        if (normals == nullptr)
        {
            Core::Utils::Logger::Error("[FBX_Importer] Can't get normal element");
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

    void FBX_Importer::GetUV(FbxMesh* mesh, std::int32_t vertexIndex, std::int32_t uvChannel, std::int32_t uvLayer, Math::float2& uv)
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
            Core::Utils::Logger::Error("[FBX_Importer] Can't get geometry element uv!");
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
        const char* nodeTypeName = node->GetTypeName();
        const char* nodeName = node->GetName();
        FbxDouble3 translation = node->LclTranslation.Get();
        FbxDouble3 rotation = node->LclRotation.Get();
        FbxDouble3 scaling = node->LclScaling.Get();					
        Core::Utils::Logger::Log("[FBX_Importer] [Node] Name: %s TypeName %s Translation: (%f, %f, %f) Rotation: (%f, %f, %f) Scale: (%f, %f, %f)",
        	nodeName, nodeTypeName,
        	translation[0], translation[1], translation[2],
        	rotation[0], rotation[1], rotation[2],
        	scaling[0], scaling[1], scaling[2]);
    }

    void FBX_Importer::Destroy()
    {
        m_manager->Destroy();
    }

    void FBX_Importer::GetMaterialIndices(FbxMesh* mesh, std::vector<std::int32_t>& meshMatIndices)
    {
        std::int32_t triangleCount = mesh->GetPolygonCount();

        fbxsdk::FbxLayerElementArrayTemplate<int>* materialIndices;
        FbxGeometryElement::EMappingMode materialMappingMode = FbxGeometryElement::eNone;

        if (mesh->GetElementMaterialCount() > 0)
        {
            materialIndices = &mesh->GetElementMaterial()->GetIndexArray();
            materialMappingMode = mesh->GetElementMaterial()->GetMappingMode();
            if (materialIndices)
            {
                switch (materialMappingMode)
                {

                case fbxsdk::FbxLayerElement::eByPolygon:
                {
                    if (materialIndices->GetCount() == triangleCount)
                    {
                        for (std::int32_t triangleIndex = 0; triangleIndex <  triangleCount; triangleIndex++)
                        {
                            std::int32_t materialIndex = materialIndices->GetAt(triangleIndex);
                            meshMatIndices.push_back(materialIndex);
                        }
                    }
                }
                break;

                case fbxsdk::FbxLayerElement::eAllSame:
                {
                    std::int32_t materialIndex = materialIndices->GetAt(0);
                    for (std::int32_t triangleIndex = 0; triangleIndex < triangleCount; triangleIndex++)
                    {
                        meshMatIndices.push_back(materialIndex);
                    }
                }
                break;

                default:
                    // TODO: Add error maybe ?
                    break;
                }
            }
        }
    }

	bool FBX_Importer::Import(std::string path, Render::Mesh* _mesh)
	{
        FbxImporter* importer = FbxImporter::Create(m_manager, "");
        // mCount = 0;
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
                    const auto nodeTranslation = node->LclTranslation.Get();
                    const auto nodeRotation = node->LclRotation.Get();

                    const auto lightgo = Scene::CreateGameObject<GameObjects::GameObject>(node->GetName(), "Light", 1,
                        Math::float3(static_cast<float>(nodeTranslation[0]),
                            static_cast<float>(nodeTranslation[1]), static_cast<float>(nodeTranslation[2])));

                    lightgo->GetTransform()->SetRotation(Math::float3(static_cast<float>(nodeRotation[0]),
                        static_cast<float>(nodeRotation[1]), static_cast<float>(nodeRotation[2])));
                    
                          
                    GameObjects::Components::Light::Light* lightc = nullptr;
                    const auto nodeLight = node->GetLight();
                    switch (nodeLight->LightType)
                    {
                        case FbxLight::EType::eVolume:
                            Core::Utils::Logger::Error("FbxLight::EType::eVolume is not implemented!");
                            lightc = lightgo->CreateComponent<GameObjects::Components::Light::Light>();
                            break;
                        case FbxLight::EType::eArea:
                            Core::Utils::Logger::Error("FbxLight::EType::eArea is not implemented!");
                            lightc = lightgo->CreateComponent<GameObjects::Components::Light::Light>();
                            break;
                        case FbxLight::EType::ePoint:
                            lightc = lightgo->CreateComponent<GameObjects::Components::Light::PointLight>();
                            break;
                        case FbxLight::EType::eDirectional:
                            lightc = lightgo->CreateComponent<GameObjects::Components::Light::DirectionalLight>();
                            break;
                        case FbxLight::EType::eSpot:
                            lightc = lightgo->CreateComponent<GameObjects::Components::Light::SpotLight>();
                            break;
                        default:
                            Core::Utils::Logger::Error("Unknown light type!");
                            lightc = lightgo->CreateComponent<GameObjects::Components::Light::Light>();
                            break;
                    }

           
                    const auto color = nodeLight->Color.Get();
                    lightc->SetColor({ static_cast<float>(color[0]), static_cast<float>(color[1]), static_cast<float>(color[2]), 1.0f });
                    
                    if (_mesh->m_meshGameObject != nullptr)
                    {
                        lightgo->GetTransform()->SetParent(_mesh->m_meshGameObject);
                    }
                }
                else if (attributeType == FbxNodeAttribute::EType::eMesh)
                {
                    std::uint32_t vertexCount = 0;
                    std::uint32_t indexCount  = 0;

                    auto mesh        = node->GetMesh();
                    auto polyCount   = mesh->GetPolygonCount();
                    auto vertices    = mesh->GetControlPoints();

                    if (vertices == nullptr)
                        continue;

                    // FIX ME: Memory is not cleaning up on fail
                    
                    _mesh->m_indexBuffers.push_back(new Render::IndexBuffer<std::uint32_t>());
                    auto indexBuffer = _mesh->m_indexBuffers[_mesh->m_countMeshes];

                    _mesh->m_vertexBuffers.push_back(new Render::VertexBuffer<Render::Vertex>());
                    auto vertexBuffer = _mesh->m_vertexBuffers[_mesh->m_countMeshes];

                    // 1. Load model stage 
                    for (std::uint32_t poly = 0; poly < polyCount; poly++)
                    {
                        auto polySize = mesh->GetPolygonSize(poly);
                        Assert(polySize == 3, "Wrong poly size!");


                        for (std::int32_t polyVert = 0; polyVert < polySize; polyVert++)
                        {
                            // Get vertex index
                            auto index = mesh->GetPolygonVertex(poly, polyVert);

                            for (std::int32_t j = 0; j < 3; j++)
                            {
                                indexBuffer->GetArray().push_back(indexCount++);
                            }

                            // Create new vertex
                            auto vertex = Vertex();

                            // Get vertex position
                            const auto currentVecPos = vertices[index];
                            vertex.position = Math::float3(static_cast<float>(currentVecPos.mData[0]),
                                static_cast<float>(currentVecPos.mData[1]), static_cast<float>(currentVecPos.mData[2]));

                            // Get uv position
                            GetUV(mesh, index, mesh->GetTextureUVIndex(poly, polyVert), 0, vertex.uv);

                            // Get normal position
                            GetNormal(mesh, index, vertexCount, vertex.normal);

                            

                            // Set color for vertex
                            auto surfaceMaterial = node->GetMaterial(i);
                            if (surfaceMaterial != nullptr)
                            {
                                if (surfaceMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
                                {
                                    const auto diffuse = ((FbxSurfacePhong*)surfaceMaterial)->Diffuse.Get();
                                    vertex.color = Math::float4(static_cast<float>(diffuse[0]),
                                        static_cast<float>(diffuse[1]), static_cast<float>(diffuse[2]), 1.0f);
                                }                              
                            }
                            else
                            {
                                vertex.color = Math::float4(1, 1, 1, 1);
                            }

                            // Push created vertex to list 
                            vertexBuffer->GetArray().push_back(vertex);
                            vertexCount++;
                        }
                    }

                    Assert(indexBuffer->Create(), "Failed to create index buffer for mesh");
                    Assert(vertexBuffer->Create(), "Failed to create vertex buffer for mesh");

                    texture_indexed_t tex = { };
                    // Save current index 
                    tex.index = _mesh->m_countMeshes;

                    // Increase mesh count 
                    _mesh->m_countMeshes += 1;

                    // 2. Load texture, material stage 
                    
                    std::int32_t numMaterials = node->GetMaterialCount();
                    for (std::int32_t matId = 0; matId < numMaterials; matId++)
                    {
                        FbxSurfaceMaterial* surfaceMaterial = node->GetMaterial(matId);

                        if (surfaceMaterial != nullptr)
                        {
                            FbxProperty prop = surfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
                            std::int32_t layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();
                            const auto resourceManager = Core::Resources::ResourceManager::GetInstance();

                            if (layeredTextureCount > 0)
                            {
                                //Logger::Log("[FBX_Importer] Load layeredTexture %d", layeredTextureCount);
                                for (std::int32_t currentLTextureCount = 0; currentLTextureCount < layeredTextureCount; currentLTextureCount++)
                                {
                                    FbxFileTexture* texture = FbxCast<FbxFileTexture>(prop.GetSrcObject<FbxTexture>(currentLTextureCount));

                                    std::string textureName = texture->GetRelativeFileName();
                                    bool replace = Core::Utils::StringAllReplace(textureName, "\\", "/");
                                    if (replace == false)
                                        continue;

                                    textureName.erase(textureName.rfind("."), textureName.length());

                                    if (resourceManager->IsExists<Render::Texture>(textureName) == false)
                                    {
                                        resourceManager->Load<Render::Texture>(textureName);
                                    }

                                    tex.texture = resourceManager->Get<Render::Texture>(textureName);
                                    _mesh->m_textures.push_back(tex);
                                }
                            }
                            else
                            {
                                // Directly get textures
                                std::int32_t textureCount = prop.GetSrcObjectCount<FbxTexture>();
                                //Logger::Log("[FBX_Importer] Load texture %d", textureCount);
                                for (std::int32_t currentTextureCount = 0; currentTextureCount < textureCount; currentTextureCount++)
                                {
                                    FbxFileTexture* texture = FbxCast<FbxFileTexture>(prop.GetSrcObject<FbxTexture>(currentTextureCount));

                                    std::string textureName = texture->GetRelativeFileName();
                                    bool replace = Core::Utils::StringAllReplace(textureName, "\\", "/");
                                    if (replace == false)
                                        continue;

                                    textureName.erase(textureName.rfind("."), textureName.length());

                                    if (resourceManager->IsExists<Render::Texture>(textureName) == false)
                                    {
                                        resourceManager->Load<Render::Texture>(textureName);
                                    }

                                    tex.texture = resourceManager->Get<Render::Texture>(textureName);
                                    _mesh->m_textures.push_back(tex);
                                }
                            }
                        }
                    }
                }
            }
        }
        scene->Destroy();
		return true;
	}

    [[nodiscard]] inline FbxManager* FBX_Importer::GetFbxManager()
    {
        return m_manager;
    }
}