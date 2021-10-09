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

		//Set special id by string 
		void						SetID(const std::string id_str);

		//Get current ID in string format 
		inline std::string			GetIDString() const;

		//Get current ID in UUID format  
		inline boost::uuids::uuid	GetUUID() const { return m_uuid; };

		static inline boost::uuids::uuid ConvertStringToUUID(const std::string id);

	private:
		boost::uuids::uuid m_uuid;
	};
}