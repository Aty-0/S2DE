#include "Skybox.h"
#include "Scene/SceneManager.h"

#include "Base/ResourceManager.h"
#include "Render/Renderer.h"
#include "Render/Buffers.h"
#include "Render/Texture.h"
#include "Render/Shader.h"

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

		const auto resourceManager = Core::Resources::ResourceManager::GetInstance();
		m_shader = new Render::Shader(*resourceManager->Get<Render::Shader>("Skybox"));
		m_texture = new Render::Texture(*resourceManager->Get<Render::Texture>("DefaultSky"));
	}

	Skybox::~Skybox()
	{
		Core::Delete(m_vertexBuffer);
		Core::Delete(m_indexBuffer);
		Core::Delete(m_shader);
		Core::Delete(m_texture);
	}

	bool Skybox::LoadTexture(std::string name)
	{
		// FIXME: Release old texture!

		Render::Texture* skyTexture = new Render::Texture();
		const auto resourceManager = Core::Resources::ResourceManager::GetInstance();

		const auto skyPath = resourceManager->GetFilePath(name, skyTexture);
		Verify(skyTexture->CreateCubeMapTexture(skyPath), "Can't create default cubemap");
		resourceManager->Add(skyTexture, name);
		m_texture = skyTexture;
		return true;
	}

	void Skybox::OnRender(Render::Renderer* renderer)
	{	 
		m_shader->UpdateMainConstBuffer(renderer, UpdateTransformation());

		// Bind shader and texture 
		m_shader->Bind(renderer);
		m_texture->Bind(renderer);

		// Bind buffers
		m_vertexBuffer->Bind(renderer);
		m_indexBuffer->Bind(renderer);

		// Draw poly 
		renderer->TurnZBufferOff();
		renderer->SetRasterizerState(Render::Api::RasterizerMode::Default);
		renderer->DrawIndexed(m_indexBuffer->GetArray().size(), 0, 0, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		renderer->TurnZBufferOn();

		// Unbind 
		m_shader->Unbind(renderer);
		m_texture->Unbind(renderer);
		m_vertexBuffer->Unbind(renderer);
		m_indexBuffer->Unbind(renderer);

	}	 
		 
	void Skybox::CreateVertexBuffer()
	{
		m_vertexBuffer->GetArray() =
		{
			// Front
			{   { -1.0f, 1.0f,-1.0f },  { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
			{	{ 1.0f, 1.0f,-1.0f },	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
			{   { -1.0f, -1.0f,-1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
			{	{ 1.0f, -1.0f,-1.0f },  { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
			// Back										  																						  
			{	{ -1.0f, 1.0f, 1.0f },  { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f} },
			{	{ 1.0f, 1.0f, 1.0f },   { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f} },
			{   { -1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f} },
			{	{ 1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f} },
		};
		Assert(m_vertexBuffer->Create(), "Failed to create index buffer");
		m_vertexBuffer->Update(Render::Renderer::GetInstance());
	}	 
		 
	void Skybox::CreateIndexBuffer()
	{	 
		m_indexBuffer->GetArray() =
		{
			// Front
			0, 1, 2,
			2, 1, 3,

			// Back
			5, 4, 7,
			7, 4, 6,

			// Right
			1, 5, 3,
			3, 5, 7,

			// Left
			4, 0, 6,
			6, 0, 2,

			// Top
			4, 5, 0,
			0, 5, 1,

			// Bottom
			2, 3, 6,
			6, 3, 7,
		};

		Assert(m_indexBuffer->Create(), "Failed to create index buffer");
		m_indexBuffer->Update(Render::Renderer::GetInstance());
	}
	 

	inline Math::float4x4 Skybox::UpdateTransformation()
	{
		const static auto transform = GetOwner()->GetTransform();
		auto wMatrix = transform->GetWorldMatrix();
		auto camPosition = Math::float3(1, 1, 1);

		const static auto camera = Scene::GetObjectByName<GameObject>(Camera::EngineCameraName)->GetComponent<Camera>();
		if (camera != nullptr)
		{
			camPosition = camera->GetOwner()->GetTransform()->GetPosition();
		}

		wMatrix = DirectX::XMMatrixTransformation(
			// Scale
			// Center | Rotation | Scaling
			Math::float3::Zero, Math::float3::Zero,
			Math::float3(20, 20, 20), // FIXME: magic size 
			// Rotation
			// Center | Quatarnion
			Math::float3::Zero, Math::float3::Zero,
			// Translation
			camPosition);

		wMatrix.Transpose(wMatrix);

		return wMatrix;
	}
}