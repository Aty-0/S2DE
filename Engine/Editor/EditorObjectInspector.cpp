#include "EditorObjectInspector.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Base/InputManager.h"
#include "Scene/SceneManager.h"
#include "Base/GameWindow.h"

#include "Render/Renderer.h"

#include "GameObjects/Base/GameObject.h"
#include "GameObjects/Components/Component.h"

#include <boost/range/adaptor/reversed.hpp>

namespace S2DE::Editor
{
	EditorObjectInspector::EditorObjectInspector() : 
		m_selectedGameObject(nullptr),
		m_selectedComponent(nullptr),
		m_showEngineGameObjects(false),
		m_uiElementsHeight(0.25f)
	{

	}

	EditorObjectInspector::~EditorObjectInspector()
	{
		
	}

	void EditorObjectInspector::Reset()
	{
		if (m_selectedComponent != nullptr)
			m_selectedComponent->Unselect();

		m_selectedComponent = nullptr;

		if(m_selectedGameObject != nullptr)
			m_selectedGameObject->Unselect();

		m_selectedGameObject = nullptr;
	}

	void EditorObjectInspector::Render(Render::Renderer* renderer)
	{
		if (!m_draw)
			return;

		// Save inspector focus 
		static bool isFocused = false;

		static const auto scene = Scene::SceneManager::GetInstance()->GetScene();

		// TODO: Lock game input 

		static const auto window = Core::GameWindow::GetInstance();
		ImGui::SetNextWindowSizeConstraints(ImVec2(300, 300), ImVec2(800, window->GetHeight()));

		ImGui::Begin("Inspector", &m_draw, ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);
		isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_::ImGuiFocusedFlags_ChildWindows);

		if (scene != nullptr)
		{
			if (ImGui::Checkbox("Show Engine Objects", &m_showEngineGameObjects))
			{
				Reset();
			}

			if (ImGui::ListBoxHeader("Objects", ImVec2(ImGui::GetWindowSize().x, 300)))
			{
				for (const auto& object : scene->GetStorage())
				{
					auto gameObject = object.second.get();

					// If iterator is empty we skip it
					if (gameObject == nullptr)
						continue;

					// If we don't need to show objects with ENGINE prefix we skip that object
					if (m_showEngineGameObjects == false && gameObject->GetPrefix() == -1)
						continue;

					//if (object.second->GetTransform()->GetParent() == nullptr)
					{
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, m_uiElementsHeight));
						if (m_selectedGameObject == gameObject)
						{
							if (ImGui::Checkbox("##", &m_selectedGameObject->m_visible))
							{
								const auto child = m_selectedGameObject->GetTransform()->GetChild();
								if (child != nullptr)
								{
									child->SetVisible(m_selectedGameObject->m_visible);
								}
							}
							ImGui::SameLine();
						}

						if (ImGui::Selectable(gameObject->GetName().c_str(), gameObject->m_isSelected))
						{
							if (m_selectedGameObject == nullptr)
							{
								m_selectedGameObject = gameObject;
								m_selectedGameObject->Select();
								//Logger::Log("[EditorObjectInspector] New object have been selected! %s", m_selectedGameObject->GetName().c_str());
							}
							else if (m_selectedGameObject != gameObject)
							{
								//Unselect previous object and select new object
								m_selectedGameObject->Unselect();
								m_selectedGameObject = gameObject;
								m_selectedGameObject->Select();
								//Logger::Log("[EditorObjectInspector] Previous object was unselected, selected new: %s", m_selectedGameObject->GetName().c_str());
							}
							else if (m_selectedGameObject == gameObject) // We are just clicked at the same object, so it's mean we are want to unselect
							{
								//Logger::Log("[EditorObjectInspector] It's same object, reset all %s", m_selectedGameObject->GetName().c_str());
								Reset();
							}

						}

						ImGui::PopStyleVar();
					}

				
				}

				ImGui::ListBoxFooter();
			}

			ImGui::Spacing();

			// TODO: Move all of this to details window
			if (m_selectedGameObject != nullptr)
			{
				ImGui::Text("Selected:");
				ImGui::Text("Name:%s ", m_selectedGameObject->GetName().c_str());
				ImGui::Text("UUID:%s ", m_selectedGameObject->GetUUIDString().c_str());

				ImGui::Text("Components:");

				if (ImGui::ListBoxHeader("Components", ImVec2(ImGui::GetWindowSize().x, 300)))
				{
					for (const auto& component : m_selectedGameObject->GetComponentsList())
					{
						if (component.second == nullptr)
							continue;

						if (ImGui::Selectable(component.second->GetName().c_str(), component.second->isSelected()))
						{
							if (m_selectedComponent == nullptr)
							{
								m_selectedComponent = component.second;
								m_selectedComponent->Select();
							}
							else if (m_selectedComponent != component.second)
							{
								// Unselect previous component and select new object
								m_selectedComponent->Unselect();
								m_selectedComponent = component.second;
								m_selectedComponent->Select();
							}
						}
					}

					ImGui::ListBoxFooter();
				}

				if (m_selectedComponent != nullptr)
				{
					ImGui::Text("Details:");
					ImGui::Text("Selected:");
					ImGui::Text("Name:%s ",	m_selectedComponent->GetName().c_str());
					ImGui::Text("UUID:%s ",	m_selectedComponent->GetUUIDString().c_str());
				}
			}


			ImGui::End();
		}
	}

	inline GameObjects::GameObject* EditorObjectInspector::GetSeletectedGameObject()
	{
		return m_selectedGameObject;
	}

	inline GameObjects::Components::Component* EditorObjectInspector::GetSeletectedComponent()
	{
		return m_selectedComponent;
	}
}