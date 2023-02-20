#include "StaticMesh.h"
#include "GameObjects/Base/GameObject.h"

namespace S2DE::GameObjects::Components
{
	StaticMesh::StaticMesh() : 
		m_mesh(nullptr),
		m_texture(nullptr),
		m_indexBuffer(nullptr),
		m_vertexBuffer(nullptr),
		m_shader(nullptr)
	{

	}

	StaticMesh::~StaticMesh()
	{
		//TODO: Need to check count of using
		if (m_unload_texture == true)
			Core::Engine::GetResourceManager().Erase<Render::Texture>(m_texture->GetName());

		Core::Delete(m_vertexBuffer);
		Core::Delete(m_indexBuffer);
		Core::Delete(m_shader);
		Core::Delete(m_texture);
		Core::Delete(m_mesh);
	}
	
	bool StaticMesh::LoadMesh(std::string name)
	{	
		//If mesh not found in resource manager storage we try to load it 
		if (!Core::Engine::GetResourceManager().IsExists<Render::Mesh>(name))
		{
			if (!Core::Engine::GetResourceManager().Load<Render::Mesh>(name))
				return false;
		}

		//Set mesh if mesh is exist
		m_mesh = new Render::Mesh(*Core::Engine::GetResourceManager().Get<Render::Mesh>(name));
		Assert(m_mesh != nullptr, "Failed to load mesh!");

		m_vertexBuffer = new Render::VertexBuffer<Render::Vertex>();
		m_indexBuffer = new Render::IndexBuffer<std::uint32_t>();
		m_vertexBuffer->GetArray() = m_mesh->GetVertices();
		m_indexBuffer->GetArray() = m_mesh->GetIndices();

		if (m_vertexBuffer->GetArray().size() == 0 || m_indexBuffer->GetArray().size() == 0)
		{
			m_vertexBuffer = nullptr;
			m_indexBuffer = nullptr;
			return false;
		}

		CreateVertexBuffer();
		CreateIndexBuffer();
		SetDefaultShader();
		SetDefaultTexture();

		m_textureCube = new Render::Texture();
		const auto cubePath = Core::Engine::GetResourceManager().GetFilePath("cubemap", m_textureCube);
		m_textureCube->CreateCubeMapTexture(cubePath);

		return true;
	}	 
		 
	bool StaticMesh::LoadTexture(std::string name)
	{	 
		return LoadTextureA(name);
	}	 
		 
	bool StaticMesh::LoadTextureA(std::string name, bool unload_texture, bool auto_load_texture)
	{	 
		//Set unload state
		m_unload_texture = unload_texture;

		//If texture not found in resource manager storage we try to load it 
		if (!Core::Engine::GetResourceManager().IsExists<Render::Texture>(name)
			&& auto_load_texture == true)
		{
			if (!Core::Engine::GetResourceManager().Load<Render::Texture>(name))
				return false;
		}
		//Set texture if texture is exist
		m_texture = new Render::Texture(*Core::Engine::GetResourceManager().Get<Render::Texture>(name));
		Assert(m_texture != nullptr, "Failed to load texture");
		return true;
	}	 
		 
	void StaticMesh::UpdateShader()
	{	 
		// Get shader name
		std::string name = m_shader->GetName();
		// Delete previous shader 
		//Core::Delete(m_shader);

		// Try to get shader by name from resource manager
		auto new_shader = Core::Engine::GetResourceManager().Get<Render::Shader>(name);

		// If shader not found
		if (new_shader == nullptr)
		{
			Logger::Error("%s Can't update shader!", GetName().c_str());
			return;
		}

		m_shader = new Render::Shader(*new_shader);
		Assert(m_shader != nullptr, "Failed to load new shader");
	}	 
		 
	void StaticMesh::UpdateTexture()
	{	 	 
		// Get texture name
		std::string name = m_texture->GetName();
		// Delete previous texture
		//Core::Delete(m_texture);

		// Try to get texture by name from resource manager
		auto new_texture = Core::Engine::GetResourceManager().Get<Render::Texture>(name);

		//If texture not found
		if (new_texture == Core::Engine::GetResourceManager().Get<Render::Texture>("DefaultTexture"))
		{
			Logger::Error("%s Can't update texture!", GetName().c_str());
			return;
		}

		m_texture = new Render::Texture(*new_texture);
		Assert(m_texture != nullptr, "Failed to load new texture");
	}	 

	void StaticMesh::SetColor(Math::Color<float> color)
	{	 
		if (m_vertexBuffer != nullptr)
		{
			m_color = color;
			//Update vertex buffer
			CreateVertexBuffer();
		}
	}	 
		 
	void StaticMesh::UseIndices(bool use)
	{
		m_useIndices = use;
	}

	void StaticMesh::OnRender()
	{	 
		if (m_vertexBuffer && m_indexBuffer)
		{
			if (m_useIndices)
			{
				// Bind and update variables in const buffer
				m_shader->UpdateMainConstBuffer(GetOwner()->GetTransform()->UpdateTransformation());

				// Bind shader and texture 
				m_shader->Bind();
				m_textureCube->Bind(1);
				m_texture->Bind();

				// Bind buffers
				m_vertexBuffer->Bind();
				m_indexBuffer->Bind();

				// Draw poly 		
				Core::Engine::GetRenderer()->SetRasterizerState("fcc");
				Core::Engine::GetRenderer()->DrawIndexed(m_indexBuffer->GetArray().size(), 0, 0, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				
				// Unbind 
				m_shader->Unbind();
				m_textureCube->Unbind();
				m_texture->Unbind();
				m_vertexBuffer->Unbind();
				m_indexBuffer->Unbind();
			}
			else
			{
				// Bind and update variables in const buffer
				m_shader->UpdateMainConstBuffer(GetOwner()->GetTransform()->UpdateTransformation());

				// Bind shader and texture 
				m_shader->Bind();
				m_textureCube->Bind(1);
				m_texture->Bind();

				// Bind buffers
				m_vertexBuffer->Bind();
				
				// Draw poly 		
				Core::Engine::GetRenderer()->SetRasterizerState("fcc");
				Core::Engine::GetRenderer()->Draw(m_vertexBuffer->GetArray().size(), 0, D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				// Unbind 
				m_shader->Unbind();
				m_textureCube->Unbind();
				m_texture->Unbind();
				m_vertexBuffer->Unbind();				
			}
		}
	}	 
		 
	void StaticMesh::CreateVertexBuffer()
	{	 
		Assert(m_vertexBuffer->Create(), "Failed to create vertex buffer");
		m_vertexBuffer->Update();
	}	 
		 
	void StaticMesh::CreateIndexBuffer()
	{	 
		Assert(m_indexBuffer->Create(), "Failed to create index buffer");
		m_indexBuffer->Update();
	}	 
		 
	void StaticMesh::SetDefaultShader()
	{	 
		m_shader = new Render::Shader(*Core::Engine::GetResourceManager().Get<Render::Shader>("Mesh"));
	}	 
		 
	void StaticMesh::SetDefaultTexture()
	{
		m_texture = new Render::Texture(*Core::Engine::GetResourceManager().Get<Render::Texture>("DefaultTexture"));
	}

}