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
		m_handle(nullptr),
		m_show_engine_object(false)
	{

	}

	EditorObjectInspector::~EditorObjectInspector()
	{
		
	}

	void EditorObjectInspector::Reset()
	{
		if(m_handle != nullptr)
			m_handle->Unselect();

		m_handle = nullptr;
		m_select_object_name.clear();
	}

	void EditorObjectInspector::Render()
	{
		// Save inspector focus 
		static bool isFocused = false;
		static std::vector<GameObjects::GameObject*> listOfChilds;
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
			if (ImGui::Checkbox("Show Engine Objects", &m_show_engine_object))
				Reset();

			if (ImGui::ListBoxHeader("##", ImVec2(ImGui::GetWindowSize().x, 300)))
			{
				static GameObjects::GameObject* withChild = nullptr;

				for (const auto& object : Core::Engine::GetSceneManager()->GetScene()->GetStorage())
				{
					// TODO: Parse child...

					// If iterator is empty we skip it
					if (object.second == nullptr)
						continue;

					// If we don't need to show objects with ENGINE prefix we skip that object
					if (m_show_engine_object == false && object.second->GetPrefix() == -1)
						continue;


					if (object.second->GetTransform()->GetParent() == nullptr)
					{
						// TODO: Get it from object 
						bool visible = false;
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

							if (m_handle == nullptr)
							{
								m_select_object_name = object.first.first;
								m_handle = object.second.get();
								m_handle->Select();
								Logger::Log("[EditorObjectInspector] New object have been selected! %s", m_handle->GetName().c_str());
							}
							else if (m_handle != object.second.get())
							{
								//Unselect previous object and select new object
								m_handle->Unselect();
								m_select_object_name = object.first.first;
								m_handle = object.second.get();
								m_handle->Select();
								Logger::Log("[EditorObjectInspector] Previous object was unselected, selected new: %s", m_handle->GetName().c_str());
							}
							else if (m_handle == object.second.get())
							{
								Logger::Log("[EditorObjectInspector] It's same object, reset all %s", m_handle->GetName().c_str());
								Reset();
							}

						}
					}

					if (m_handle == object.second.get())
					{
						if (m_handle == withChild
							&& withChild != nullptr)
						{
							float size = 5.0f;
							for (auto go : listOfChilds)
							{
								ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(size, size));
								if (ImGui::TreeNode(go->GetName().c_str()))
								{
									size += 10.0f;
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
			if (m_handle != nullptr)
			{
				ImGui::Text("Selected:%s", m_handle->GetName().c_str());
				ImGui::Text("Components:");

				if (ImGui::ListBoxHeader("2", ImVec2(ImGui::GetWindowSize().x, 300)))
				{
					for (const auto& component : m_handle->GetComponentsList())
					{
						if (component.second == nullptr)
							continue;

						if (ImGui::Selectable(component.second->GetName().c_str(), component.second->isSelected()))
						{
							if (m_component == nullptr)
							{
								m_component = component.second;
								m_component->Select();
							}
							else if (m_component != component.second)
							{
								//Unselect previous object and select new object
								m_component->Unselect();
								m_component = component.second;
								m_component->Select();
							}
						}
					}

					ImGui::ListBoxFooter();
				}

				if (m_component != nullptr)
				{
					ImGui::Text("Details:");
					ImGui::Text("No details for you, son.");
					ImGui::Text("TODO: Draw it");
				}
			}


			ImGui::End();
		}
	}
}