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

	void EditorCenterCursor::UpdatePosition()
	{
		const static auto window = Core::GameWindow::GetInstance();
		const auto transform = GetOwner()->GetTransform();
		transform->SetPosition({ static_cast<float>(window->GetWidth() / 2.0f), static_cast<float>(window->GetHeight() / 2.0f), 0.0f });
	}

	void EditorCenterCursor::OnCreate()
	{	
		const auto owner = GetOwner();
		m_sprite = owner->CreateComponent<Sprite>();
		m_sprite->SetUIMode(true);
		m_sprite->LoadTexture("cursor");

		const auto alpha = owner->CreateComponent<Components::AlphaComponent>();
		alpha->SetAlpha(true);

		const auto window = Core::GameWindow::GetInstance();
		window->onWindowResized.AddCallback(std::bind(&EditorCenterCursor::UpdatePosition, this));
	}

	void EditorCenterCursor::OnRender(Render::Renderer* renderer)
	{
		// TODO: Can be removed, test animation 
		static const float speed = 20.0f;
		static const auto time = Core::GameTime::GetInstance();
		static float x = 0.0f;
		x += speed * time->GetDeltaTime();

		static const auto transform = GetOwner()->GetTransform();
		transform->SetRotationX(x);

		m_sprite->OnRender(renderer);
	}
}