#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/UUID.h"
#include "Base/Utils/CallbackWrapper.h"

namespace S2DE::GameObjects
{
	class GameObject;
	namespace Components
	{
		class Transform;
	}
}

namespace S2DE::Render
{
	class Renderer;
}

namespace S2DE::GameObjects::Components
{
	class S2DE_API Component : public Core::Utils::UUID
	{
	public:
		Component();
		~Component();

		[[nodiscard]] inline std::string GetName() const;
		[[nodiscard]] inline std::uint32_t GetPriority() const;
		[[nodiscard]] inline GameObject* GetOwner() const;
		[[nodiscard]] inline bool isSelected() const;

		virtual void				 Select() { m_isSelected = true; }
		virtual void				 Unselect() { m_isSelected = false; }

		virtual void				 OnCreate() { }
		virtual void				 OnDestroy() { }
		virtual void				 OnRender(Render::Renderer* renderer) { }
		virtual void				 OnUpdate(float deltaTime) { }

		// What's we are render when we are select the game object
		virtual void				 OnRenderSelected(Render::Renderer* renderer, Transform* transform);

		void						 SetPriority(std::uint32_t priority);
	private:
		std::string			m_name;
		std::uint32_t		m_priority;
		GameObject*			m_owner;
		bool				m_isSelected;
	protected:
		// Needed for setting instance pointer safely
		friend GameObject;

		void						 SetOwner(GameObject* go);
		void						 SetName(std::string name);

	private:
		S2DE_SERIALIZE_BASE_BEGIN();
		S2DE_SERIALIZE_ADD(m_name);
		S2DE_SERIALIZE_ADD(m_priority);
		S2DE_SERIALIZE_ADD(m_isSelected);
		S2DE_SERIALIZE_ADD(m_owner); 
		S2DE_SERIALIZE_END();

	};
}

S2DE_SERIALIZE_CLASS_IMPLEMENTATION(S2DE::GameObjects::Components::Component, boost::serialization::object_serializable);