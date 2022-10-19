#pragma once
#include "Base/Main/Common.h"
#include "Render/Renderer.h"
#include "Render/Buffers.h"

#include "Render/FR/Shader.h"
#include "GameObjects/Components/UI/UIDrawableComponent.h"

namespace S2DE::GameObjects::Components::Editor
{
	class S2DE_API EditorCenterCursor : public UI::UIDrawableComponent
	{
	public:
		EditorCenterCursor();
		virtual ~EditorCenterCursor();

		virtual void OnRender() override;
		virtual void OnCreate() override;

	private:
		Render::VertexBuffer<Render::Vertex>* m_vertex_buffer;
		Render::FR::Shader* m_shader;
	};
}
