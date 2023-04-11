#include "EditorCenterCursor.h"

#include "GameObjects/Base/GameObject.h"
#include "GameObjects/Components/Camera.h"
#include "GameObjects/Components/AlphaComponent.h"
#include "GameObjects/Components/Sprite.h"

namespace S2DE::GameObjects::Components::Editor
{
	EditorCenterCursor::EditorCenterCursor()
	{

	}

	EditorCenterCursor::~EditorCenterCursor()
	{

	}

	void EditorCenterCursor::OnCreate()
	{	
		m_sprite = GetOwner()->CreateComponent<Sprite>();
		m_sprite->SetUIMode(true);
		m_sprite->LoadTexture("cursor");
		auto alpha = GetOwner()->CreateComponent<Components::AlphaComponent>();
		alpha->SetAlpha(true);
		GetOwner()->GetTransform()->SetScale(DirectX::SimpleMath::Vector3(7, 7, 1));
	}

	void EditorCenterCursor::OnRender(Render::Renderer* renderer)
	{
		m_sprite->OnRender(renderer);
	}
}