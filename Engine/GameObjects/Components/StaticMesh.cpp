#include "StaticMesh.h"

#include "Scene/SceneManager.h"
#include "GameObjects/Base/GameObject.h"

#include "Base/ResourceManager.h"

#include "Render/Renderer.h"
#include "Render/Buffers.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Render/Mesh.h"
#include "Render/CB.h"

#include "Math/Color.h"

namespace S2DE::GameObjects::Components
{
	StaticMesh::StaticMesh() :
		m_mesh(nullptr),
		m_shader(nullptr),
		m_useSkyCube(true),
		m_useIndices(true),
		m_savedIndex(0)
	{

	}

	StaticMesh::~StaticMesh()
	{
		Core::Delete(m_shader);
		Core::Delete(m_mesh);
	}

	void StaticMesh::CutMeshToParts()
	{
		if (m_mesh != nullptr)
		{
			// Current object is a head 
			m_savedIndex = 0;

			// Do not try to create sub objects if we are have 1 or less meshes
			if (m_mesh->GetCountMeshes() <= 1)
				return;

			const auto transform = GetOwner()->GetTransform();

			for (std::uint32_t i = 0; i < m_mesh->GetCountMeshes(); i++)
			{
				static GameObject* prMeshPartObject = nullptr;

				// TODO: Get name of node
				const auto meshPartObject = Scene::CreateGameObject<GameObjects::GameObject>(GetName() + std::to_string(i));


				meshPartObject->GetTransform()->SetPosition(transform->GetPosition());
				meshPartObject->GetTransform()->SetRotation(transform->GetRotation());
				meshPartObject->GetTransform()->SetParent(GetOwner());
				
				
				const auto meshPartSMComponent = meshPartObject->CreateComponent<StaticMesh>();
				meshPartSMComponent->m_mesh = m_mesh;
				meshPartSMComponent->m_shader = m_shader;
				meshPartSMComponent->m_textureCube = m_textureCube;
				meshPartSMComponent->m_useSkyCube = true;
				meshPartSMComponent->m_savedIndex = i;

				prMeshPartObject = meshPartObject;
			}
		}
	}

	// TODO: Change shader, skycube 
	bool StaticMesh::LoadMesh(std::string name)
	{	
		const auto resourceManager = Core::Resources::ResourceManager::GetInstance();
		// If mesh not found in resource manager storage we try to load it 
		if (!resourceManager->IsExists<Render::Mesh>(name))
		{
			m_mesh = new Render::Mesh();
			m_mesh->SetMeshGameObject(GetOwner());
			m_mesh->Load(name);

			if (!resourceManager->Add<Render::Mesh>(m_mesh))
				return false;
		}
		else
		{
			// Set mesh if mesh is exist
			m_mesh = new Render::Mesh(*resourceManager->Get<Render::Mesh>(name));
			Assert(m_mesh != nullptr, "Failed to load mesh!");
		}

		m_shader = new Render::Shader(*resourceManager->Get<Render::Shader>("Mesh"));
		if (m_useSkyCube)
		{
			m_textureCube = resourceManager->Get<Render::Texture>("DefaultCubemap");
		}


		CutMeshToParts();

		return true;
	}	 
		 
	bool StaticMesh::LoadTexture(std::string name)
	{	 
		S2DE_NO_IMPL();
		return true;
	}	 
		 
	void StaticMesh::UpdateShader()
	{	 
		if (m_shader == nullptr)
		{
			return;
		}
		
		const auto resourceManager = Core::Resources::ResourceManager::GetInstance();

		// Get shader name
		const auto name = m_shader->GetName();

		// Try to get shader by name from resource manager
		const auto new_shader = resourceManager->Get<Render::Shader>(name);

		// If shader not found
		if (new_shader == nullptr)
		{
			Core::Utils::Logger::Error("%s Can't update shader!", GetName().c_str());
			return;
		}

		m_shader = new Render::Shader(*new_shader);
		Assert(m_shader != nullptr, "Failed to load new shader");
	}	 
		 
	void StaticMesh::UpdateTexture()
	{	 	 
		if (m_mesh == nullptr)
		{
			return;
		}

		const auto resourceManager = Core::Resources::ResourceManager::GetInstance();

		auto tex = m_mesh->GetTextures();

		if (tex.size() != 0)
		{
			for (std::uint32_t j = m_savedIndex; j < tex.size(); j++)
			{
				auto currentTexture = tex[j];
				if (currentTexture.index == m_savedIndex)
				{
					// Get texture name
					std::string name = currentTexture.texture->GetName();
					// Delete previous texture
					//Core::Delete(m_texture);

					// Try to get texture by name from resource manager
					auto new_texture = resourceManager->Get<Render::Texture>(name);

					// If texture not found
					if (new_texture == resourceManager->Get<Render::Texture>("DefaultTexture"))
					{
						Core::Utils::Logger::Error("%s Can't update texture!", GetName().c_str());
						return;
					}

					currentTexture.texture = new Render::Texture(*new_texture);
					Assert(currentTexture.texture != nullptr, "Failed to load new texture");
				}
			}
		}

	}	 

	void StaticMesh::SetColor(Math::Color<float> color)
	{	 
		S2DE_NO_IMPL();
		/*
		if (m_vertexBuffer != nullptr)
		{
			m_color = color;
			CreateVertexBuffer();
		}
		*/
	}	 
		 
	void StaticMesh::UseIndices(bool use)
	{
		m_useIndices = use;
	}

	void StaticMesh::OnRender(Render::Renderer* renderer)
	{	
		// Mesh is invalid, we can't continue...
		if (m_mesh == nullptr)
			return;

		auto transform = GetOwner()->GetTransform();
		auto vBuff = m_mesh->GetVertexBuffers()[m_savedIndex];
		auto iBuff = m_mesh->GetIndexBuffers()[m_savedIndex];
		auto tex = m_mesh->GetTextures();

		if (tex.size() != 0)
		{
			for (std::uint32_t j = m_savedIndex; j < tex.size(); j++)
			{
				if (tex[j].index == m_savedIndex)
				{
					tex[j].texture->Bind(renderer, 3);
					break;
				}
			}
		}

		if (m_useSkyCube)
		{
			m_textureCube->Bind(renderer, 1);
		}

		// Bind and update variables in const buffer
		m_shader->UpdateMainConstBuffer(renderer, transform->UpdateTransformation());

		// Bind shader and texture 
		m_shader->Bind(renderer);

		// Bind buffers
		vBuff->Bind(renderer);
		if (m_useIndices)
		{
			iBuff->Bind(renderer);
		}

		// Draw poly 		
		renderer->SetRasterizerState(Render::Api::RasterizerMode::TwoSided);

		if (m_useIndices)
		{
			renderer->DrawIndexed(iBuff->GetArray().size(), 0, 0, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
		else
		{
			renderer->Draw(vBuff->GetArray().size(), 0, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		// Unbind 
		if (tex.size() != 0)
		{
			for (std::uint32_t j = m_savedIndex; j < tex.size(); j++)
			{
				if (tex[j].index == m_savedIndex)
				{
					tex[j].texture->Unbind(renderer, 3);
					break;
				}
			}
		}

		if (m_useSkyCube)
		{
			m_textureCube->Unbind(renderer, 1);
		}

		m_shader->Unbind(renderer);

		vBuff->Unbind(renderer);

		if (m_useIndices)
		{
			iBuff->Unbind(renderer);
		}

		
	}	 	
}