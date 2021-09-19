#pragma once
#include "Base/Main/Common.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/GameObjectIDGenerator.h"

namespace S2DE
{
	typedef std::vector<std::pair<std::pair<std::string, boost::uuids::uuid>, 
		GameObject*>> SceneObjectStorage;

	class S2DE_API Scene
	{
	public:
		Scene();
		~Scene();
		

		void				 Rename(std::string object_name, std::string new_object_name);
		void				 Rename(boost::uuids::uuid object_id, std::string new_object_name);

		void				 Delete(std::string object_name);
		void				 Delete(boost::uuids::uuid object_id);

		void				 Replace(std::string object_name, GameObject* object);
		void				 Replace(boost::uuids::uuid object_id, GameObject* object);

		void				 Clone(std::string object_name);
		void				 Clone(boost::uuids::uuid object_id);

		void				 Clear();
		void				 Destroy();

		void				 Add(GameObject* g);


		//Check object name on exist
		//if name existing we add number of same object
		void				CheckNameOnExist(std::string& name);

		inline SceneObjectStorage&		GetStorage() { return m_storage; }
		inline std::string				GetName() const { return m_name; }
	private:
		SceneObjectStorage m_storage;
		std::string m_name;
	};
}
