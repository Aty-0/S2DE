#include "EditorCenterCursor.h"
#include "Base/Engine.h"
#include "Base/InputManager.h"
#include "Base/ResourceManager.h"
#include "Base/GameWindow.h"
#include "Scene/SceneManager.h"

#include "GameObjects/Components/Camera.h"

namespace S2DE::GameObjects::Components::Editor
{
	EditorCenterCursor::EditorCenterCursor()
	{
		m_vertex_buffer = new Render::VertexBuffer<Render::Vertex>();
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


		Assert(Core::Engine::GetResourceManager().Load<Render::Shader>("editor_cursor"), "Cursor shader is not loaded!");
		m_shader = new Render::Shader(*Core::Engine::GetResourceManager().Get<Render::Shader>("editor_cursor"));

	}

	EditorCenterCursor::~EditorCenterCursor()
	{
		Core::Delete(m_vertex_buffer);
		Core::Delete(m_shader);
	}

	void EditorCenterCursor::OnCreate()
	{
		GetOwner()->GetTransform()->SetScale(DirectX::SimpleMath::Vector3(2, 2, 2));
		GetOwner()->GetTransform()->SetRotation_X(45.0f);
	}

	void EditorCenterCursor::OnRender()
	{
		m_shader->UpdateMainConstBuffer(GetOwner()->GetTransform()->UpdateTransformation(), true);
		m_vertex_buffer->Bind();
		m_shader->Bind();
		Core::Engine::GetRenderer()->Draw(m_vertex_buffer->GetArray().size(), 0, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		m_shader->Unbind();
		m_vertex_buffer->Unbind();
	}
}