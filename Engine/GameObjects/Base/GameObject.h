#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/UUID.h"
#include "Base/Utils/Logger.h"
#include "GameObjects/Components/Component.h"
#include "GameObjects/Components/Transform.h"


#define S2DE_UUID_REGENERATE "R"
#define S2DE_DEFAULT_GAMEOBJECT_NAME "GameObject"
#define S2DE_DEFAULT_GAMEOBJECT_TYPE "Default"
#define S2DE_DEFAULT_GAMEOBJECT_PREFIX 0
#define	S2DE_ENGINE_GAMEOBJECT_TYPE "_Engine_"

namespace S2DE::Render
{
	class Renderer;
}

namespace S2DE::Editor
{
	class EditorObjectInspector;
}

namespace S2DE::GameObjects
{
	namespace Components
	{
		class Sprite;
	}

	typedef std::vector<std::pair<std::pair<boost::uuids::uuid, std::type_index>, Components::Component*>> ComponentsList;
	class S2DE_API GameObject : public Core::Utils::UUID
	{
	public:
		friend class S2DE::Editor::EditorObjectInspector;

		GameObject();
		explicit GameObject(std::string name, std::string type, std::int32_t prefix, std::string id = std::string());
		virtual ~GameObject();

		void Render(Render::Renderer* renderer);
		void Update(float deltaTime);
		void SetName(std::string name);
		void SetPrefix(std::int32_t prefix);
		void SetType(std::string type);
		void SetVisible(bool visible);
		void SetEnabled(bool enabled);
		
		virtual void Select() { m_isSelected = true; }
		virtual void Unselect() { m_isSelected = false; }

		[[nodiscard]] inline std::string  GetName()   const;
		[[nodiscard]] inline std::int32_t GetPrefix() const;
		[[nodiscard]] inline std::string  GetType()   const;
		[[nodiscard]] inline bool isVisible() const;
		[[nodiscard]] inline bool isEnabled() const;
		[[nodiscard]] inline bool isSelected() const;
		[[nodiscard]] inline Components::Transform* GetTransform() const;
		[[nodiscard]] inline ComponentsList GetComponentsList() const;

		[[nodiscard]] inline Components::Sprite* GetIcon()   const;

	private:
		std::string					 m_name; 
		std::int32_t				 m_prefix;
		std::string					 m_type;
		bool						 m_enabled;
		bool						 m_visible;
		bool						 m_isSelected;
		Components::Transform*		 m_transform;
		Components::Sprite*			 m_objectIconSprite;

	public:
		template<typename T>
		T* AddComponent(T* component, std::uint32_t priority = 0);

		// TODO: delete by priority ?
		template<typename T>
		void DeleteComponent();

		template<typename T>
		T* CreateComponent(std::uint32_t priority = 0);

		template<typename T>
		[[nodiscard]] T* GetComponent();

		template<typename T>
		[[nodiscard]] T* GetComponentInChildren();
			
		template<typename T>
		[[nodiscard]] T* GetComponentInParent();

	private:
		ComponentsList m_components;
	};
}
#include "GameObjects/Base/GameObject_inline.h"
