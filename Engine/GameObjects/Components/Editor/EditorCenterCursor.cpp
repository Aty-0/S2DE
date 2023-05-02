#include "EditorCenterCursor.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "Base/GameTime.h"

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
		const auto owner = GetOwner();
		m_sprite = owner->CreateComponent<Sprite>();
		m_sprite->SetUIMode(true);
		m_sprite->LoadTexture("cursor");

		const auto alpha = owner->CreateComponent<Components::AlphaComponent>();
		alpha->SetAlpha(true);
	}

	void EditorCenterCursor::OnRender(Render::Renderer* renderer)
	{
		const auto transform = GetOwner()->GetTransform();
		const auto window = Core::Engine::GetGameWindow();
		transform->SetPosition({ static_cast<float>(window->GetWidth() / 2.0f), static_cast<float>(window->GetHeight() / 2.0f), 0.0f });
		static float x = 0.0f;
		static const float speed = 20.0f;
		x += speed * Core::Engine::GetGameTime().GetDeltaTime();

		transform->SetRotation_X(x);
		m_sprite->OnRender(renderer);
	}
}