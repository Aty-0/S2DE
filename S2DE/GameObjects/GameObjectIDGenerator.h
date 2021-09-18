#pragma once
#include "Base/Main/Common.h"

#include <boost/uuid/uuid.hpp>

namespace S2DE
{
	class S2DE_API GameObjectIDGenerator
	{
	public:
		GameObjectIDGenerator();
		~GameObjectIDGenerator();

		//Set special id in string format
		void				SetID(const std::string id_str);
		//Get corrent ID in string format 
		std::string			GetIDString() const;
		//Get corrent ID in UUID 
		boost::uuids::uuid	GetUUID() const { return m_uuid; };

	private:
		boost::uuids::uuid m_uuid;
	};
}