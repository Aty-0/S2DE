#include "FBX_Importer.h"
#include "Base/Engine.h"

namespace S2DE::Render
{
	FbxManager* FBX_Importer::m_manager;

	void FBX_Importer::Init()
	{
		m_manager = FbxManager::Create();
        Core::Utils::Logger::Log("FBX SDK Version: %s", m_manager->GetVersion());

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

    void FBX_Importer::ProcessUV(FbxMesh* mesh, std::int32_t vertexIndex, std::int32_t uvChannel, std::int32_t uvLayer, DirectX::SimpleMath::Vector2& uv)
    {
        if (mesh->GetElementUVCount() == 0)        
            return;
        
        if (uvLayer > 1 || mesh->GetElementUVCount() <= uvLayer)
            return;

        FbxGeometryElementUV* vertexUV = mesh->GetElementUV(uvLayer);

        switch (vertexUV->GetMappingMode())
        {
        case FbxGeometryElement::eByControlPoint:
            switch (vertexUV->GetReferenceMode())
            {
            case FbxGeometryElement::eDirect:
            {
                uv.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(vertexIndex).mData[0]);
                uv.y = 1 - static_cast<float>(vertexUV->GetDirectArray().GetAt(vertexIndex).mData[1]);
            }
            break;

            case FbxGeometryElement::eIndexToDirect:
            {
                std::int32_t index = vertexUV->GetIndexArray().GetAt(vertexIndex);
                uv.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
                uv.y = 1 - static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
            }
            break;

            default:
                Core::Utils::Logger::Error("Invalid reference");
                return;
            }
            break;

        case FbxGeometryElement::eByPolygonVertex:
            switch (vertexUV->GetReferenceMode())
            {
            case FbxGeometryElement::eDirect:
            case FbxGeometryElement::eIndexToDirect:
            {
                uv.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(uvChannel).mData[0]);
                uv.y = 1 - static_cast<float>(vertexUV->GetDirectArray().GetAt(uvChannel).mData[1]);
            }
            break;

            default:
                Core::Utils::Logger::Error("Invalid reference");
                return;
            }
            break;

        case FbxGeometryElement::eNone:
        case FbxGeometryElement::eByPolygon:
        case FbxGeometryElement::eByEdge:
        case FbxGeometryElement::eAllSame:
            Core::Utils::Logger::Error("Unhandled mapping mode for texture coordinate");
            break;
        }
    }


    void FBX_Importer::Destroy()
    {
        m_manager->Destroy();
    }

	bool FBX_Importer::Import(std::string path, FbxScene*& scene)
	{
        FbxImporter* importer = FbxImporter::Create(m_manager, "");

		if (!importer->Initialize(path.c_str(), -1, m_manager->GetIOSettings()))
		{
			Core::Utils::Logger::Error("Can't import this model %s", path.c_str());
			return false;
		}

		scene = FbxScene::Create(m_manager, "ImportScene");

        if (!importer->Import(scene))
            return false;
        
		if (!importer->IsFBX())
			return false;

        if (scene->GetGlobalSettings().GetSystemUnit() != FbxSystemUnit::m)
            FbxSystemUnit::m.ConvertScene(scene);

        FbxGeometryConverter conventer(m_manager);
        conventer.Triangulate(scene, true, false);

        importer->Destroy();
		return true;
	}
}