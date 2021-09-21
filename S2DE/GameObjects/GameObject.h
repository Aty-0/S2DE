#pragma once
#include "Base/Main/Common.h"
#include "GameObjects/Transform.h"
#include "GameObjects/GameObjectIDGenerator.h"

namespace S2DE
{
	class S2DE_API GameObject : public Transform
	{
	public:
		GameObject();
		~GameObject();

		void						 Render();
		void						 Update(float DeltaTime);
		void						 UpdateInput();
		void						 RenderDebugGUI();

		//Object initialize  
		//Set name, id or generate it, type, prefix
		void						Init(std::string name, std::string type, std::int32_t prefix, std::string id = std::string());

		void						SetName(std::string name);
		void						SetPrefix(std::int32_t prefix);
		void						SetType(std::string type);
		void						SetVisible(bool visible);
		void						SetEnabled(bool enabled);

		inline std::string          GetName()   const { return m_name; }
		inline boost::uuids::uuid   GetUUID()	 const { return m_id->GetUUID(); }
		inline std::string          GetID()     const { return m_id->GetIDString(); }
		inline std::string          GetType()   const { return m_type; }
		inline bool                 isVisible() const { return m_visible; }
		inline bool                 isEnabled() const { return m_enabled; }
		template<typename T>
		T*							Clone() const { return new T(*this); }
		template<typename T>
		static T*					Create() { return new T(); }


	protected:
		virtual void				OnPositionChanged()  override { }
		virtual void				OnRotationChanged()  override { }
		virtual void				OnScaleChanged()     override { }
		virtual void				OnCreate() { }
		virtual void				OnDestroy() { }
		virtual void				OnRender() { }
		virtual void				OnUpdate(float DeltaTime) { }
		virtual void				OnUpdateInput() { }
		virtual void				OnDebugRenderGUI() { }

	private:
		std::string					m_name; 
		std::int32_t				m_prefix;
		GameObjectIDGenerator*		m_id;
		std::string					m_type;
		bool						m_enabled;
		bool						m_visible;
	};

}