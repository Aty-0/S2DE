#include "Skybox.h"
#include "Scene/SceneManager.h"

#include "GameObjects/Base/GameObject.h"
#include "GameObjects/Components/Camera.h"

namespace S2DE::GameObjects::Components
{
	Skybox::Skybox() :
		m_texture(nullptr),
		m_shader(nullptr),
		m_indexBuffer(new Render::IndexBuffer<std::int32_t>()),
		m_vertexBuffer(new Render::VertexBuffer<Render::Vertex>())
	{
		CreateVertexBuffer();
		CreateIndexBuffer();
		SetDefaultTexture();
		SetDefaultShader();
	}

	Skybox::~Skybox()
	{
		Core::Delete(m_vertexBuffer);
		Core::Delete(m_indexBuffer);
		Core::Delete(m_shader);
		Core::Delete(m_texture);
	}

	bool Skybox::LoadTextureA(std::string name, bool unload_texture, bool auto_load_texture)
	{
		Logger::Warning("LoadTextureA method is not usable for Skybox!");
		return LoadTexture(name);
	}

	bool Skybox::LoadTexture(std::string name)
	{
		const auto path = Core::Engine::GetResourceManager().GetFilePath(name, m_texture);
		return m_texture->CreateCubeMapTexture(path);
	}

	void Skybox::OnRender()
	{	 
		m_shader->UpdateMainConstBuffer(UpdateTransformation());

		//Bind shader and texture 
		m_shader->Bind();
		m_texture->Bind();

		//Bind buffers
		m_vertexBuffer->Bind();
		m_indexBuffer->Bind();

		//Draw poly 
		Core::Engine::GetRenderer()->TurnZBufferOff();
		Core::Engine::GetRenderer()->SetRasterizerState("nocull");
		Core::Engine::GetRenderer()->DrawIndexed(m_indexBuffer->GetArray().size(), 0, 0, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Core::Engine::GetRenderer()->SetRasterizerState();
		Core::Engine::GetRenderer()->TurnZBufferOn();
		//Unbind 
		m_shader->Unbind();
		m_texture->Unbind();
		m_vertexBuffer->Unbind();
		m_indexBuffer->Unbind();

	}	 
		 
	void Skybox::CreateVertexBuffer()
	{
		m_vertexBuffer->GetArray() =
		{
			//Front
			{   DirectX::SimpleMath::Vector3(-1.0f, 1.0f,-1.0f),  DirectX::SimpleMath::Vector4(1,1,1,1), DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
			{	DirectX::SimpleMath::Vector3(1.0f, 1.0f,-1.0f),	  DirectX::SimpleMath::Vector4(1,1,1,1), DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
			{   DirectX::SimpleMath::Vector3(-1.0f, -1.0f,-1.0f), DirectX::SimpleMath::Vector4(1,1,1,1), DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
			{	DirectX::SimpleMath::Vector3(1.0f, -1.0f,-1.0f),  DirectX::SimpleMath::Vector4(1,1,1,1), DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
			//Back										  																						  
			{	DirectX::SimpleMath::Vector3(-1.0f, 1.0f, 1.0f),  DirectX::SimpleMath::Vector4(1,1,1,1), DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
			{	DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f),   DirectX::SimpleMath::Vector4(1,1,1,1), DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
			{   DirectX::SimpleMath::Vector3(-1.0f, -1.0f, 1.0f), DirectX::SimpleMath::Vector4(1,1,1,1), DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
			{	DirectX::SimpleMath::Vector3(1.0f, -1.0f,  1.0f), DirectX::SimpleMath::Vector4(1,1,1,1), DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
		};
		Assert(m_vertexBuffer->Create(), "Failed to create index buffer");
		m_vertexBuffer->Update();
	}	 
		 
	void Skybox::CreateIndexBuffer()
	{	 
		m_indexBuffer->GetArray() =
		{
			//Front
			0, 1, 2,
			2, 1, 3,

			//Back
			5, 4, 7,
			7, 4, 6,

			//Right
			1, 5, 3,
			3, 5, 7,

			//Left
			4, 0, 6,
			6, 0, 2,

			//Top
			4, 5, 0,
			0, 5, 1,

			//Bottom
			2, 3, 6,
			6, 3, 7,
		};

		Assert(m_indexBuffer->Create(), "Failed to create index buffer");
		m_indexBuffer->Update();
	}
	 
	void Skybox::SetDefaultShader()
	{	 
		Assert(Core::Engine::GetResourceManager().Load<Render::Shader>("Skybox"), "Failed to load sky shader");

		m_shader = new Render::Shader(*Core::Engine::GetResourceManager().Get<Render::Shader>("Skybox"));
	}	 
		 
	void Skybox::SetDefaultTexture()
	{
		m_texture = new Render::Texture();
		Assert(LoadTexture("sky"), "Failed to load default sky texture");
	}

	inline DirectX::SimpleMath::Matrix Skybox::UpdateTransformation()
	{
		const auto transform = GetOwner()->GetTransform();

		auto wMatrix = transform->GetWorldMatrix();
		auto camera = Scene::GetObjectByName<GameObject>(S2DE_MAIN_CAMERA_NAME)->GetComponent<Camera>();
		auto camPosition = DirectX::SimpleMath::Vector3(1, 1, 1);

		if (camera != nullptr)
		{
			camPosition = camera->GetOwner()->GetTransform()->GetPosition();
		}

		wMatrix = DirectX::XMMatrixTransformation(
			//Scale
			//Center | Rotation | Scaling
			DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3(20, 20, 20),
			//Rotation
			//Center | Quatarnion
			DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero,
			//Translation
			camPosition);

		wMatrix.Transpose(wMatrix);

		return wMatrix;
	}
}