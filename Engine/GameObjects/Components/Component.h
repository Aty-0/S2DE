#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/UUID.h"

namespace S2DE::GameObjects
{
	class S2DE_API GameObject;
}

namespace S2DE::GameObjects::Components
{
	class S2DE_API Component : public Core::Utils::UUID
	{
	public:
		Component();
		virtual ~Component();

		inline std::string			 GetName() const { return m_name; }
		inline std::uint32_t		 GetPriority() const { return m_priority; }
		inline GameObject*			 GetOwner() const { return m_owner; }

		virtual void				 OnCreate() { }
		virtual void				 OnDestroy() { }
		virtual void				 OnRender() { }
		virtual void				 OnUpdate(float deltaTime) { }

		void						 SetPriority(std::uint32_t priority);
	private:
		std::string			m_name;
		std::uint32_t		m_priority;
		GameObject*			m_owner;

	protected:
		// Needed for setting instance pointer safely
		friend GameObject;

		void						 SetOwner(GameObject* go);
		void						 SetName(std::string name);
	};
}