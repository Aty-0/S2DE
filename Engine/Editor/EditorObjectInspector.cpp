#include "EditorObjectInspector.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Base/InputManager.h"
#include "Scene/SceneManager.h"
#include "Base/GameWindow.h"

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

	void EditorObjectInspector::Render()
	{
		// Save inspector focus 
		static bool isFocused = false;
		static std::vector<GameObjects::GameObject*> listOfChilds;

		Scene::SceneManager* sceneManager = Core::Engine::GetSceneManager();

		// Lock all control stuff if window is active
		Core::Engine::GetInputManager()->LockKeyboardControl(isFocused && m_draw);
		Core::Engine::GetInputManager()->LockMouseControl(isFocused && m_draw);
		Core::Engine::GetInputManager()->LockWheel(isFocused && m_draw);

		if (!m_draw)
			return;

		ImGui::SetNextWindowSizeConstraints(ImVec2(300, 300), ImVec2(800, Core::Engine::GetGameWindow()->GetHeight()));

		ImGui::Begin("Inspector", &m_draw, ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);
		isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_::ImGuiFocusedFlags_ChildWindows);

		if (Core::Engine::GetSceneManager()->GetScene() != nullptr)
		{
			if (ImGui::Checkbox("Show Engine Objects", &m_showEngineGameObjects))
			{
				Reset();
			}

			if (ImGui::ListBoxHeader("Objects", ImVec2(ImGui::GetWindowSize().x, 300)))
			{
				static GameObjects::GameObject* withChild = nullptr;

				for (const auto& object : Core::Engine::GetSceneManager()->GetScene()->GetStorage())
				{
					// If iterator is empty we skip it
					if (object.second == nullptr)
						continue;

					// If we don't need to show objects with ENGINE prefix we skip that object
					if (m_showEngineGameObjects == false && object.second->GetPrefix() == -1)
						continue;

					if (object.second->GetTransform()->GetParent() == nullptr)
					{
						// TODO: Get it from object 
						bool visible = true;
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, m_uiElementsHeight));
						ImGui::Checkbox("##", &visible);
						ImGui::SameLine();
						if (ImGui::Selectable(object.first.first.c_str(), object.second->isSelected()))
						{
							withChild = nullptr;
							listOfChilds.clear();
							listOfChilds.shrink_to_fit();

							if (object.second->GetTransform()->GetChild() != nullptr)
							{
								withChild = object.second.get();

								GameObjects::GameObject* current = withChild->GetTransform()->GetChild();

								while (current != nullptr)
								{
									listOfChilds.push_back(current);

									if (current != nullptr)
									{
										Logger::Log("[EditorObjectInspector] Save: %s", current->GetName().c_str());
									}

									current = current->GetTransform()->GetChild();
								}
							}

							if (m_selectedGameObject == nullptr)
							{
								m_selectedGameObject = object.second.get();
								m_selectedGameObject->Select();
								Logger::Log("[EditorObjectInspector] New object have been selected! %s", m_selectedGameObject->GetName().c_str());
							}
							else if (m_selectedGameObject != object.second.get())
							{
								//Unselect previous object and select new object
								m_selectedGameObject->Unselect();
								m_selectedGameObject = object.second.get();
								m_selectedGameObject->Select();
								Logger::Log("[EditorObjectInspector] Previous object was unselected, selected new: %s", m_selectedGameObject->GetName().c_str());
							}
							else if (m_selectedGameObject == object.second.get())
							{
								Logger::Log("[EditorObjectInspector] It's same object, reset all %s", m_selectedGameObject->GetName().c_str());
								Reset();
							}

						}

						ImGui::PopStyleVar();
					}


					if (m_selectedGameObject == object.second.get())
					{
						if (m_selectedGameObject == withChild
							&& withChild != nullptr)
						{
							float posX = 5.0f;
							for (auto go : listOfChilds)
							{
								ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(posX, m_uiElementsHeight));
								if (ImGui::TreeNode(go->GetName().c_str()))
								{
									posX += 10.0f;

									// TODO: Select child
									if (ImGui::Selectable(go->GetName().c_str(),
										go->isSelected()))
									{

									}

									ImGui::PopStyleVar();
									ImGui::TreePop();
								}
								else
								{
									ImGui::PopStyleVar();
									break;
								}
							}
						}
					}
				}

				ImGui::ListBoxFooter();
			}

			ImGui::Spacing();

			// TODO: Move all of this to details window
			if (m_selectedGameObject != nullptr)
			{
				ImGui::Text("Selected:%s %s", m_selectedGameObject->GetName().c_str(), m_selectedGameObject->GetUUIDString().c_str());
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
					ImGui::Text("Selected:%s %s", m_selectedComponent->GetName().c_str(), m_selectedComponent->GetUUIDString().c_str());
					ImGui::Text("No details for you, son.");
					ImGui::Text("TODO: Draw it");
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