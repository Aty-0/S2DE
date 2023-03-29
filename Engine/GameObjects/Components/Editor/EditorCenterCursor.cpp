#include "EditorCenterCursor.h"

#include "GameObjects/Base/GameObject.h"
#include "GameObjects/Components/Camera.h"
#include "GameObjects/Components/AlphaComponent.h"

namespace S2DE::GameObjects::Components::Editor
{
	EditorCenterCursor::EditorCenterCursor()
	{
		/*m_vertex_buffer = new Render::VertexBuffer<Render::Vertex>();
		m_vertex_buffer->GetArray() =
		{
			{ DirectX::SimpleMath::Vector3(-1.0f,   -1.0f,	0.0f), DirectX::SimpleMath::Vector4(255, 255, 255, 255),  DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
			{ DirectX::SimpleMath::Vector3(0.0f,   0.0f,	0.0f), DirectX::SimpleMath::Vector4(255, 255, 255, 255),  DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
			{ DirectX::SimpleMath::Vector3(1.0f,   -1.0f,	0.0f), DirectX::SimpleMath::Vector4(255, 255, 255, 255),  DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
			{ DirectX::SimpleMath::Vector3(-1.0f,   1.0f,	0.0f), DirectX::SimpleMath::Vector4(255, 255, 255, 255),  DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
			{ DirectX::SimpleMath::Vector3(0.0f,   0.0f,	0.0f), DirectX::SimpleMath::Vector4(255, 255, 255, 255),  DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
			{ DirectX::SimpleMath::Vector3(1.0f,   1.0f,	0.0f), DirectX::SimpleMath::Vector4(255, 255, 255, 255),  DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
		};

		Assert(m_vertex_buffer->Create(), "Failed to create vertex buffer!");
		m_vertex_buffer->Update();

		m_shader = new Render::Shader(*Core::Engine::GetResourceManager().Get<Render::Shader>("editor_cursor"));
		*/
	}

	EditorCenterCursor::~EditorCenterCursor()
	{
		/*
		Core::Delete(m_vertex_buffer);
		Core::Delete(m_shader);
		*/
	}

	void EditorCenterCursor::OnCreate()
	{
		/*
		GetOwner()->GetTransform()->SetRotation_X(45.0f);
		*/
		
		m_sprite = GetOwner()->CreateComponent<Sprite>();
		m_sprite->SetUIMode(true);
		m_sprite->LoadTexture("cursor");
		auto alpha = GetOwner()->CreateComponent<Components::AlphaComponent>();
		alpha->SetAlpha(true);
		GetOwner()->GetTransform()->SetScale(DirectX::SimpleMath::Vector3(7, 7, 1));
	}

	void EditorCenterCursor::OnRender()
	{
		m_sprite->OnRender();
		/*
		m_shader->UpdateMainConstBuffer(GetOwner()->GetTransform()->UpdateTransformation(), true);
		m_vertex_buffer->Bind();
		m_shader->Bind();
		Core::Engine::GetRenderer()->Draw(m_vertex_buffer->GetArray().size(), 0, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		m_shader->Unbind();
		m_vertex_buffer->Unbind();
		*/
	}
}