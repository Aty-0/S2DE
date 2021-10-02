#pragma once
#include "Base/Main/Common.h"
#include "GameObjects/Transform.h"
#include "GameObjects/GameObjectIDGenerator.h"

//TODO
//Prefix


#define S2DE_DEFAULT_GAMEOBJECT_NAME "BasicGameObjectName"
#define S2DE_DEFAULT_GAMEOBJECT_TYPE "BasicGameObjectType"
#define S2DE_DEFAULT_GAMEOBJECT_PREFIX 0

namespace S2DE
{
	class S2DE_API GameObject : public Transform
	{
	public:
		GameObject();
		~GameObject();

		//Object initialization  
		//Set name, type, prefix
		//ID will be automatically genarated but we can set custom id
		void						 Init(std::string name, std::string type, std::int32_t prefix, std::string id = std::string());

		void						 Render();
		void						 Update(float DeltaTime);
		void						 UpdateInput();
		void						 RenderDebugGUI();

		void						 SetName(std::string name);
		void						 SetPrefix(std::int32_t prefix);
		void						 SetType(std::string type);
		void						 SetVisible(bool visible);
		void						 SetEnabled(bool enabled);
									 
		inline std::string           GetName()   const { return m_name; }
		inline boost::uuids::uuid    GetUUID()	 const { return m_id->GetUUID(); }
		inline std::string           GetID()     const { return m_id->GetIDString(); }
		inline std::int32_t			 GetPrefix() const { return m_prefix; }
		inline std::string           GetType()   const { return m_type; }
		inline bool                  isVisible() const { return m_visible; }
		inline bool                  isEnabled() const { return m_enabled; }
									 
		template<typename T>		 
		T*							 Clone() const { return new T(*this); }
		

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
		virtual bool 				CheckOnIntersection() { return false; }
	private:
		std::string					m_name; 
		std::int32_t				m_prefix;
		GameObjectIDGenerator*		m_id;
		std::string					m_type;
		bool						m_enabled;
		bool						m_visible;
		bool						m_isSelected;
	};

}