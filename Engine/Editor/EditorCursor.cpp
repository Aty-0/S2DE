#include "EditorCursor.h"
#include "Base/Engine.h"
#include "Base/InputManager.h"
#include "Base/ResourceManager.h"
#include "Base/GameWindow.h"
#include "GameObjects/Camera.h"
#include "Scene/SceneManager.h"

#define CURSOR_CIRCLE_RINGS_COUNT 16
#define CURSOR_DEFAULT_SPEED 0.03f
#define CURSOR_DEFAULT_SIZE 1.1f

namespace S2DE::Editor
{
	EditorCursor::EditorCursor()
	{
		Core::Utils::Logger::Log("Create editor cursor...");

		m_cross_vertex_buffer = new Render::VertexBuffer<Render::Vertex>();
		m_cross_vertex_buffer->GetArray() =
		{
			{ DirectX::SimpleMath::Vector3(-1.0f,   -1.0f,	0.0f), DirectX::SimpleMath::Vector4(255, 255, 255, 255),  DirectX::SimpleMath::Vector2(0.0f, 0.0f) }, 
			{ DirectX::SimpleMath::Vector3(0.0f,   0.0f,	0.0f), DirectX::SimpleMath::Vector4(255, 255, 255, 255),  DirectX::SimpleMath::Vector2(0.0f, 0.0f) }, 
			{ DirectX::SimpleMath::Vector3(1.0f,   -1.0f,	0.0f), DirectX::SimpleMath::Vector4(255, 255, 255, 255),  DirectX::SimpleMath::Vector2(0.0f, 0.0f) }, 
			{ DirectX::SimpleMath::Vector3(-1.0f,   1.0f,	0.0f), DirectX::SimpleMath::Vector4(255, 255, 255, 255),  DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
			{ DirectX::SimpleMath::Vector3(0.0f,   0.0f,	0.0f), DirectX::SimpleMath::Vector4(255, 255, 255, 255),  DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
			{ DirectX::SimpleMath::Vector3(1.0f,   1.0f,	0.0f), DirectX::SimpleMath::Vector4(255, 255, 255, 255),  DirectX::SimpleMath::Vector2(0.0f, 0.0f) },
		};

		S2DE_ASSERT(m_cross_vertex_buffer->Create());
		m_cross_vertex_buffer->Update();

		m_circle_vertex_buffer = new Render::VertexBuffer<Render::Vertex>();

		for (std::int32_t i = 0; i <= CURSOR_CIRCLE_RINGS_COUNT; i++)
		{
			Render::Vertex v = 
			{ 
				DirectX::SimpleMath::Vector3((float)cos( 2 * i * 3.14 / CURSOR_CIRCLE_RINGS_COUNT), (float)sin(2 * i * 3.14 / CURSOR_CIRCLE_RINGS_COUNT), 0.0f),
				DirectX::SimpleMath::Vector4(255, 255, 255, 255),
				DirectX::SimpleMath::Vector2(0.0f, 0.0f)
			};

			m_circle_vertex_buffer->GetArray().push_back(v);
		}

		S2DE_ASSERT(m_circle_vertex_buffer->Create());
		m_circle_vertex_buffer->Update();

		S2DE_ASSERT(Core::Engine::GetResourceManager().Load<Render::Shader>("editor_cursor"));
		m_shader = new Render::Shader(*Core::Engine::GetResourceManager().Get<Render::Shader>("editor_cursor"));

		SetScale(DirectX::SimpleMath::Vector3(CURSOR_DEFAULT_SIZE, CURSOR_DEFAULT_SIZE, 1.0f));
	}

	EditorCursor::~EditorCursor()
	{
		Core::Delete(m_cross_vertex_buffer);
		Core::Delete(m_circle_vertex_buffer);
		Core::Delete(m_shader);
	}

	void EditorCursor::OnRenderIn2D()
	{
		//Rotate everything
		//* Core::Engine::GetGameTime().GetDeltaTime()
		SetRotation_X(GetRotation().x + CURSOR_DEFAULT_SPEED);

		//Calculate cursor world position 
		POINT point{};
		GetCursorPos(&point);
		ScreenToClient(Core::Engine::GetGameWindow()->GetHWND(), &point);
		SetPosition(DirectX::SimpleMath::Vector3(
			((2 * point.x) / (float)Core::Engine::GetGameWindow()->GetWidthFixed() - 1.0f) * -1.0f,
			((2 * point.y) / (float)Core::Engine::GetGameWindow()->GetHeightFixed() - 1.0f) * -1.0f,
			0.0f));

		//Adjust the points using the projection matrix to account for the aspect ratio of the viewport
		DirectX::SimpleMath::Matrix proj = Scene::GetObjectByName<GameObjects::Camera>(S2DE_MAIN_CAMERA_NAME)->GetOrthoMatrix();
		SetPosition(DirectX::SimpleMath::Vector3(-GetPosition().x / proj._11, GetPosition().y / proj._22, 0.0f));

		//Render the cross
		m_cross_vertex_buffer->Bind();
		Core::Engine::GetRenderer()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		m_shader->UpdateMainConstBuffer(UpdateTransformation(), true);
		m_shader->Bind();
		Core::Engine::GetRenderer()->GetContext()->Draw(m_cross_vertex_buffer->GetArray().size(), 0);
		m_shader->Unbind();

		//Render the circle
		m_circle_vertex_buffer->Bind();
		Core::Engine::GetRenderer()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		m_shader->UpdateMainConstBuffer(UpdateTransformation(), true);
		m_shader->Bind();
		Core::Engine::GetRenderer()->GetContext()->Draw(m_circle_vertex_buffer->GetArray().size(), 0);
		m_shader->Unbind();
	}
}