#include "EditorCenterCursor.h"
#include "Base/Engine.h"
#include "Base/InputManager.h"
#include "Base/ResourceManager.h"
#include "Base/GameWindow.h"
#include "GameObjects/Camera.h"
#include "Scene/SceneManager.h"

namespace S2DE::Editor
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

		S2DE_ASSERT(m_vertex_buffer->Create());
		m_vertex_buffer->Update();


		S2DE_ASSERT(Core::Engine::GetResourceManager().Load<Render::Shader>("editor_cursor"));
		m_shader = new Render::Shader(*Core::Engine::GetResourceManager().Get<Render::Shader>("editor_cursor"));

		SetScale(DirectX::SimpleMath::Vector3(1.8f, 1.8f, 1.0f));
	}

	EditorCenterCursor::~EditorCenterCursor()
	{
		Core::Delete(m_vertex_buffer);
		Core::Delete(m_shader);
	}

	void EditorCenterCursor::OnRenderIn2D()
	{
		m_vertex_buffer->Bind();
		Core::Engine::GetRenderer()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		m_shader->UpdateMainConstBuffer(UpdateTransformation(), true);
		m_shader->Bind();
		Core::Engine::GetRenderer()->GetContext()->Draw(m_vertex_buffer->GetArray().size(), 0);
		m_shader->Unbind();
	}
}