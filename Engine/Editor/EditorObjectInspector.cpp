#include "EditorObjectInspector.h"
#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Base/InputManager.h"
#include "Scene/SceneManager.h"
#include "Base/GameWindow.h"
#include "Math/IntVector.h"
#include "Math/Vector.h"

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
		if (!m_draw)
			return;

		ImGui::SetNextWindowSizeConstraints(ImVec2(300, 300), ImVec2(800, Core::Engine::GetGameWindow()->GetHeight()));

		ImGui::Begin("Scene object inspector", &m_draw, ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);

		//Lock all control stuff if window is active
		Core::Engine::GetInputManager()->LockKeyboardControl(ImGui::IsWindowFocused(ImGuiFocusedFlags_::ImGuiFocusedFlags_ChildWindows));
		Core::Engine::GetInputManager()->LockMouseControl(ImGui::IsWindowFocused(ImGuiFocusedFlags_::ImGuiFocusedFlags_ChildWindows));


		if (Core::Engine::GetSceneManager()->GetScene() != nullptr)
		{
			if (ImGui::Checkbox("Show Engine Objects", &m_show_engine_object))
				Reset();

			if (m_handle != nullptr)
				ImGui::Text("Selected:%s", m_handle->GetName().c_str());

			if (ImGui::ListBoxHeader("", ImVec2(0.0f, (ImGui::GetWindowSize().y / 2) + 50.0f)))
			{
				for (Scene::SceneObjectStorage::iterator it = Core::Engine::GetSceneManager()->GetScene()->GetStorage().begin();
					it != Core::Engine::GetSceneManager()->GetScene()->GetStorage().end();
					it++)
				{
					//If iterator is empty we skip it
					if (it == Core::Engine::GetSceneManager()->GetScene()->GetStorage().end())
						continue;

					//If we don't need to show objects with ENGINE prefix we skip that object
					if (m_show_engine_object == false && it->second->GetPrefix() == -1)
						continue;

					if (ImGui::Selectable(it->first.first.c_str(), it->second->isSelected()))
					{
						if (m_handle == nullptr)
						{
							m_select_object_name = it->first.first;
							m_handle = it->second.get();
							m_handle->Select();
						}
						else if (m_handle != it->second.get())
						{
							//Unselect previous object and select new object
							m_handle->Unselect();
							m_select_object_name = it->first.first;
							m_handle = it->second.get();
							m_handle->Select();
						}
					}

				}
				ImGui::ListBoxFooter();
			}

			//TODO
			//Recreate

			/*if (m_handle != nullptr)
			{
				ImGui::Separator();
				for (const auto& CurrentCategory : m_handle->SerializableCategoryList)
				{
					if (ImGui::CollapsingHeader(CurrentCategory.c_str(), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
					{
						for (const auto& CurrentProperty : m_handle->SerializablePropertiesList)
						{
							if (CurrentProperty.category == CurrentCategory)
							{
								switch (CurrentProperty.type)
								{
									case Core::ObjectPropertyTypes::D_Bool:
									{
										if (CurrentProperty.read_only == true || m_handle->GetPrefix() == -1)
											ImGui::Text("%s:%s ", CurrentProperty.name.c_str(), *reinterpret_cast<bool*>(CurrentProperty.data) == true ? "true" : "false");
										else
											ImGui::Checkbox(CurrentProperty.name.c_str(), reinterpret_cast<bool*>(CurrentProperty.data));
							
										break;
									}
									case Core::ObjectPropertyTypes::D_Float:
									{
										if (CurrentProperty.read_only == true || m_handle->GetPrefix() == -1)
											ImGui::Text("%s:%f ", CurrentProperty.name.c_str(), *reinterpret_cast<float*>(CurrentProperty.data));
										else
											ImGui::InputFloat(CurrentProperty.name.c_str(), reinterpret_cast<float*>(CurrentProperty.data));
										break;
									}
									case Core::ObjectPropertyTypes::D_Int32:
									{
										if (CurrentProperty.read_only == true || m_handle->GetPrefix() == -1)
											ImGui::Text("%s:%d ", CurrentProperty.name.c_str(), *reinterpret_cast<std::int32_t*>(CurrentProperty.data));
										else
											ImGui::InputInt(CurrentProperty.name.c_str(), reinterpret_cast<std::int32_t*>(CurrentProperty.data));
										break;
									}
									case Core::ObjectPropertyTypes::D_UInt32:
									{
										if (CurrentProperty.read_only == true || m_handle->GetPrefix() == -1)
											ImGui::Text("%s:%d ", CurrentProperty.name.c_str(), *reinterpret_cast<std::int32_t*>(CurrentProperty.data));
										else
											ImGui::InputInt(CurrentProperty.name.c_str(), reinterpret_cast<std::int32_t*>(CurrentProperty.data));
										break;
									}
									case Core::ObjectPropertyTypes::C_IntVector2:
									{
										Math::IntVector2 v_int_vec2 = *reinterpret_cast<Math::IntVector2*>(CurrentProperty.data);
										ImGui::Text("%s:%d %d", CurrentProperty.name.c_str(), v_int_vec2.x , v_int_vec2.y);
										break;
									}
									case Core::ObjectPropertyTypes::C_IntVector3:
									{
										Math::IntVector3 v_int_vec3 = *reinterpret_cast<Math::IntVector3*>(CurrentProperty.data);
										ImGui::Text("%s:%d %d %d", CurrentProperty.name.c_str(), v_int_vec3.x, v_int_vec3.y, v_int_vec3.z);
										break;
									}
									case Core::ObjectPropertyTypes::C_IntVector4:
									{
										Math::IntVector4 v_int_vec4 = *reinterpret_cast<Math::IntVector4*>(CurrentProperty.data);
										ImGui::Text("%s:%d %d %d %d", CurrentProperty.name.c_str(), v_int_vec4.x, v_int_vec4.y, v_int_vec4.z, v_int_vec4.w);
										break;
									}
									case Core::ObjectPropertyTypes::C_Vector2:
									{
										Math::Vector2 v_f_vec2 = *reinterpret_cast<Math::Vector2*>(CurrentProperty.data);
										ImGui::Text("%s:%f %f", CurrentProperty.name.c_str(), v_f_vec2.x, v_f_vec2.y);
										break;
									}
									case Core::ObjectPropertyTypes::C_Vector3:
									{
										Math::Vector3 v_f_vec3 = *reinterpret_cast<Math::Vector3*>(CurrentProperty.data);
										ImGui::Text("%s:%f %f %f", CurrentProperty.name.c_str(), v_f_vec3.x, v_f_vec3.y, v_f_vec3.z);
										break;
									}
									case Core::ObjectPropertyTypes::C_Vector4:
									{
										Math::Vector4 v_f_vec4 = *reinterpret_cast<Math::Vector4*>(CurrentProperty.data);
										ImGui::Text("%s:%f %f %f %f", CurrentProperty.name.c_str(), v_f_vec4.x, v_f_vec4.y, v_f_vec4.z, v_f_vec4.w);
										break;
									}
									case Core::ObjectPropertyTypes::D_String:
									{
										if (CurrentProperty.read_only == true || m_handle->GetPrefix() == -1)
											ImGui::Text("%s:%s ", CurrentProperty.name.c_str(), *reinterpret_cast<std::string*>(CurrentProperty.data)->c_str());
										else
										{
											char buf[1024] = "";
											std::strncpy(buf,
												reinterpret_cast<std::string*>(CurrentProperty.data)->c_str(),
												1024 - 1);

											ImGui::InputText(CurrentProperty.name.c_str(), buf, IM_ARRAYSIZE(buf));

											*reinterpret_cast<std::string*>(CurrentProperty.data) = buf;
										}
										break;
									}
									default:
										ImGui::Text("%s [Unknown type]", CurrentProperty.name);
										break;
								}

							}

						}
					}
				}
			}*/

			ImGui::End();
		}
	}
}