#pragma once
#include "Scene/SceneManager.h"
namespace S2DE::Scene
{
	template<typename T>
	[[nodiscard]] inline T* SceneManager::GetObjectByName(std::string name)
	{
		static_assert(!std::is_base_of<T, GameObjects::GameObject>::value || std::is_same<T, GameObjects::GameObject>::value, "This is not GameObject or GameObject based class");

		const auto scene = GetScene();
		Assert(scene != nullptr, "Scene is nullptr");

		auto storage = scene->GetStorage();
		SceneObjectStorage::iterator it = std::find_if(
			storage.begin(), storage.end(),
			[&name](std::pair<std::pair<std::string, boost::uuids::uuid>,
				std::shared_ptr<GameObjects::GameObject>> const& elem) {
					return elem.first.first == name;
			});

		return dynamic_cast<T*>(it->second.get());
	}

	template<typename T>
	[[nodiscard]] inline T* SceneManager::GetObjectByUUID(boost::uuids::uuid uuid)
	{
		static_assert(!std::is_base_of<T, GameObjects::GameObject>::value || std::is_same<T, GameObjects::GameObject>::value, "This is not GameObject or GameObject based class");
	
		const auto scene = GetScene();
		Assert(scene != nullptr, "Scene is nullptr");

		auto storage = scene->GetStorage();
		SceneObjectStorage::iterator it = std::find_if(
			storage.begin(), storage.end(),
			[&uuid](std::pair<std::pair<std::string, boost::uuids::uuid>,
				std::shared_ptr<GameObjects::GameObject>> const& elem) {
					return elem.first.second == uuid;
			});

		return dynamic_cast<T*>(it->second.get());
	}

	template<typename T>
	[[nodiscard]] inline T* SceneManager::CreateGameObject(std::string name, std::string type, std::int32_t prefix, Math::float3 position, Math::float3 rotation, Math::float3 scale)
	{
		static_assert(!std::is_base_of<T, GameObjects::GameObject>::value || std::is_same<T, GameObjects::GameObject>::value,
			"This is not GameObject or GameObject based class");

		const auto scene = GetScene();

		// Without scene we can't continue 
		Assert(scene != nullptr, "Scene is nullptr");

		const auto object = new T(name, type, prefix, std::string());
		const auto transform = object->GetTransform();

		// If we get there assertion fail it means something very bad happened. 
		Assert(transform, "Transform component is nullptr, wat ?");

		transform->SetPosition(position);
		transform->SetRotation(rotation);
		transform->SetScale(scale);

		return scene->Add<T>(object);
	}

	template<typename T>
	[[nodiscard]] inline T* SceneManager::CreateGameObjectNoInit()
	{
		static_assert(!std::is_base_of<T, GameObjects::GameObject>::value || std::is_same<T, GameObjects::GameObject>::value,
			"This is not GameObject or GameObject based class");

		const auto scene = GetScene();
		// Without scene we can't continue 
		Assert(scene != nullptr, "Scene is nullptr");

		const auto object = new T();
		return scene->Add<T>(object);
	}
}