#pragma once
#include "Base/Main/Common.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/GameObjectIDGenerator.h"

namespace S2DE
{
	//SceneObjectStorage type
	typedef std::vector<std::pair<std::pair<std::string, boost::uuids::uuid>, 
		GameObject*>> SceneObjectStorage;

	class S2DE_API Scene
	{
	public:
		Scene();
		~Scene();
		
		//Rename object (by name)
		void							Rename(std::string object_name, std::string new_object_name);
		//Rename object (by id)
		void							Rename(boost::uuids::uuid object_id, std::string new_object_name);

		//Delete game object (by name)
		void							Delete(std::string object_name);
		//Delete game object (by id)
		void							Delete(boost::uuids::uuid object_id);

		//Replace game object (by name)
		void							Replace(std::string object_name, GameObject* object);
		//Replace game object (by id)
		void							Replace(boost::uuids::uuid object_id, GameObject* object);

		//Clone game object (by name)
		void							Clone(std::string object_name);
		//Clone game object (by id)
		void							Clone(boost::uuids::uuid object_id);

		//Clear all scene
		//NOTE: Object with engine prefix (-1) will not been deleted
		void							Clear();
		//Destroy scene
		void							Destroy();

		//Add game object to scene
		void							Add(GameObject* g);


		//Check object name on exist
		//if name existing we add number of same object
		void							CheckNameOnExist(std::string& name);

		inline SceneObjectStorage&		GetStorage() { return m_storage; }
		inline std::string				GetName() const { return m_name; }
	private:
		SceneObjectStorage m_storage;
		std::string m_name;
	};
}
