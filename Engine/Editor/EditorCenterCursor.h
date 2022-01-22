#pragma once
#include "Base/Main/Common.h"
#include "Graphics/Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Buffers.h"
#include "GameObjects/UI/UI_Drawable.h"

namespace S2DE::Editor
{
	class EditorCenterCursor : public GameObjects::UI::UI_Drawable
	{
	public:
		EditorCenterCursor();
		~EditorCenterCursor();

	protected:
		virtual void OnRenderIn2D() override;

	private:
		Render::VertexBuffer<Render::Vertex>* m_vertex_buffer;
		Render::Shader* m_shader;
	};
}
