#pragma once
#include "Base/Main/Common.h"
#include "GameObjects/Transform.h"
#include "GameObjects/GameObjectIDGenerator.h"

//TODO
//Prefix


#define S2DE_DEFAULT_GAMEOBJECT_NAME "BasicGameObjectName"
#define S2DE_DEFAULT_GAMEOBJECT_TYPE "BasicGameObjectType"
#define S2DE_DEFAULT_GAMEOBJECT_PREFIX 0
#define	S2DE_ENGINE_GAMEOBJECT_TYPE "_Engine"

namespace S2DE
{
	class S2DE_API GameObject : public Transform
	{
	public:
		GameObject();
		virtual ~GameObject();

		//Object initialization 
		//Set name, type, prefix
		//ID automatically genarated when object is created 
		//but we can regenerate UUID or type custom uuid
		//if id string is empty it's does nothing
		//if we type REGENERATE we get new uuid
		void						 Init(std::string name, std::string type, std::int32_t prefix, std::string id = std::string());

		//Main gameobject render function
		void						 Render();

		//Main gameobject update function
		void						 Update(float DeltaTime);

		//Main gameobject input update function
		void						 UpdateInput();
		void						 RenderDebugGUI();

		//Set new name for gameobject
		void						 SetName(std::string name);

		//Set new prefix for gameobject
		void						 SetPrefix(std::int32_t prefix);

		//Set new type for gameobject
		void						 SetType(std::string type);

		//Set visible condition for gameobject
		void						 SetVisible(bool visible);

		//Set enabled condition for gameobject
		void						 SetEnabled(bool enabled);
									 
		inline std::string           GetName()   const { return m_name; }
		inline boost::uuids::uuid    GetUUID()	 const { return m_id->GetUUID(); }
		inline std::string           GetUUIDString()     const { return m_id->GetUUIDString(); }
		inline std::int32_t			 GetPrefix() const { return m_prefix; }
		inline std::string           GetType()   const { return m_type; }
		inline bool                  isVisible() const { return m_visible; }
		inline bool                  isEnabled() const { return m_enabled; }
		inline bool                  isSelected() const { return m_isSelected; }
		
		virtual void				 Select() { m_isSelected = true; }
		virtual void				 Unselect() { m_isSelected = false; }

	protected:
		virtual void				 OnPositionChanged()  override { }
		virtual void				 OnRotationChanged()  override { }
		virtual void				 OnScaleChanged()     override { }
		virtual void				 OnCreate() { }
		virtual void				 OnDestroy() { }
		virtual void				 OnRender() { }
		virtual void				 OnUpdate(float DeltaTime) { }
		virtual void				 OnUpdateInput() { }
		virtual void				 OnDebugRenderGUI() { }
		virtual bool 				 CheckOnIntersection() { return false; }

	private:
		std::string					 m_name; 
		std::int32_t				 m_prefix;
		GameObjectIDGenerator*		 m_id;
		std::string					 m_type;
		bool						 m_enabled;
		bool						 m_visible;
		bool						 m_isSelected;
	};

}