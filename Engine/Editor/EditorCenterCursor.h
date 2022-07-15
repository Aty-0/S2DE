#pragma once
#include "Base/Main/Common.h"
#include "Graphics/Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Buffers.h"
#include "GameObjects/UI/UI_Drawable.h"

namespace S2DE::Editor
{
	class S2DE_API EditorCenterCursor : public GameObjects::UI::UI_Drawable
	{
	public:
		EditorCenterCursor();
		virtual ~EditorCenterCursor();

	protected:
		virtual void OnRender() override;
		virtual void OnCreate() override;

	private:
		Render::VertexBuffer<Render::Vertex>* m_vertex_buffer;
		Render::Shader* m_shader;
	};
}
