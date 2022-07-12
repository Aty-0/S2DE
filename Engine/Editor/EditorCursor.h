#pragma once
#include "Base/Main/Common.h"
#include "Graphics/Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Buffers.h"
#include "GameObjects/UI/UI_Drawable.h"

namespace S2DE::Editor
{
	class EditorCursor : public GameObjects::UI::UI_Drawable
	{
	public:
		EditorCursor();
		~EditorCursor();

	protected:
		virtual void OnRenderIn2D() override;
		virtual void OnUpdate(float DeltaTime) override;

	private:
		Render::VertexBuffer<Render::Vertex>* m_cross_vertex_buffer;
		Render::VertexBuffer<Render::Vertex>* m_circle_vertex_buffer;
		Render::Shader* m_shader;
	};
}